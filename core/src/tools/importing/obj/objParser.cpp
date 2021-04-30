#include "gravpch.h"
#include "objParser.h"
#include "utils/stringUtils.h"
#include "exceptions/importException.h"

static const std::string s_DefaultObjectName = "defaultObject";

void GRAVEngine::Tools::Importing::objParser::parseFile()
{
	// Make sure the file is open for input
	GRAV_ASSERT(m_File.isInput());
	// Seek to the beginning of the file
	m_File.seekRead(0, IO::seekOrigin::beg);

	// Keep reading the file
	std::string line;
	while (m_File.readLine(line, '\n'))
	{
		std::vector<std::string> tokens;

		// Parse the line
		switch (line.at(0))
		{
		case 'v':
		{
			if (line.at(1) == ' ') // Vertex
			{
				Utils::trimSplit(line.substr(2), " ", tokens);
				if (tokens.size() == 3)
				{
					// Vertex Definition
					getVec3(m_Model->m_Vertices, tokens);
				}
				else if (tokens.size() == 4)
				{
					// Vertex with homogeneous
					getHomogeneousVec3(m_Model->m_Vertices, tokens);
				}
				else if (tokens.size() == 6)
				{
					// Vertex with colors
					getVec3(m_Model->m_Vertices, tokens);

					// Get a new tokens vector starting from the third item
					auto it = tokens.begin();
					for (size_t i = 0; i < 3; i++)
						it++;
					
					tokenArray tokens2 = tokenArray(it, tokens.end());

					// Vertex color definition
					getVec3(m_Model->m_VertexColors, tokens);
				}
			}
			if (line.at(1) == 't') // Texture Coordinate
			{
				Utils::trimSplit(line.substr(3), " ", tokens);
				// Get the texture coordinate and the dimensions of the texture coordinate
				uint8 textDim = getTextureCoordinate(m_Model->m_TextCoords, tokens);
				//m_Model->m_TextureCoordDim = std::max(m_Model->m_TextureCoordDim, textDim);
			}
			if (line.at(1) == 'n') // Vertex Normal
			{
				Utils::trimSplit(line.substr(3), " ", tokens);
				// Get a vertex normal
				getVec3(m_Model->m_Normals, tokens);
			}
			break;
		}
		case 'p':
		{
			// Get a face with the point type
			Utils::trimSplit(line.substr(2), " ", tokens);
			getFace(primitiveType::POINT, tokens);
			break;
		}
		case 'l':
		{
			// Get a face with the line type
			Utils::trimSplit(line.substr(2), " ", tokens);
			getFace(primitiveType::LINE, tokens);
			break;
		}
		case 'f': 
		{
			// Get a face with the ngon type
			Utils::trimSplit(line.substr(2), " ", tokens);
			getFace(primitiveType::NGON, tokens);
			break;
		}

		case '#':
		{
			break;
		}
		case 'u':
		{
			break;
		}
		case 'm':
		{
			break;
		}
		case 'g':
		{
			// Get a group name
			Utils::trimSplit(line.substr(2), " ", tokens);
			getGroupName(tokens);
			break;
		}
		case 's':
		{
			// Get a group number
			Utils::trimSplit(line.substr(2), " ", tokens);
			getGroupNumber(tokens);
			break;
		}
		case 'o':
		{
			Utils::trimSplit(line.substr(2), " ", tokens);
			getObjectName(tokens);
			break;
		}
		default:
			break;
		}
	}
}

void GRAVEngine::Tools::Importing::objParser::getHomogeneousVec3(std::vector<vec3>& pointArray, tokenArray& tokens)
{
	// Get the point
	vec3 point = {
		std::atof(tokens[0].c_str()),
		std::atof(tokens[1].c_str()),
		std::atof(tokens[2].c_str())
	};
	// Get the w component
	double w = std::atof(tokens[3].c_str());

	if (w == 0)
		throw Exceptions::importException("OBJ: Invalid component in homogeneous vector (Division by 0)");

	// Divide by homogeneous coordinate
	point /= w;

	// Add the point to the array
	pointArray.push_back(point);
}
void GRAVEngine::Tools::Importing::objParser::getVec3(std::vector<vec3>& pointArray, tokenArray& tokens)
{
	// Get the point
	vec3 point = {
		std::atof(tokens[0].c_str()),
		std::atof(tokens[1].c_str()), 
		std::atof(tokens[2].c_str())
	};

	// Add the point to the array
	pointArray.push_back(point);
}
void GRAVEngine::Tools::Importing::objParser::getVec2(std::vector<vec2>& pointArray, tokenArray& tokens)
{
	// Get the point
	vec2 point = {
		std::atof(tokens[0].c_str()),
		std::atof(tokens[1].c_str())
	};

	// Add the point to the array
	pointArray.push_back(point);
}

void GRAVEngine::Tools::Importing::objParser::getFace(primitiveType type, tokenArray& tokens)
{
	// Create a new face
	ref<objFace> newFace = createRef<objFace>(type);

	bool hasNormal = false;
	tokenArray vertexTokens;

	vertexIndex vertIndex;
	textureIndex textIndex;
	normalIndex normIndex;

	// For every token (This includes the 3 items of vertex/texture/normal)
	for (auto it = tokens.begin(); it != tokens.end(); it++)
	{
		// Split the entire vertex by the '/' character
		vertexTokens.clear();
		Utils::trimSplit((*it), "/", vertexTokens);

		if (vertexTokens.size() > 3)
			throw Exceptions::importException("OBJ: Too many vertex tokens.");
		if (type == primitiveType::POINT && vertexTokens.size() != 1)
			// There should only be one value when the primitive type is a point
			GRAV_LOG_LINE_ERROR("OBJ: Separator found in point primitive type.");

		// There will always be a vertex index
		vertIndex = std::atoi(vertexTokens[0].c_str());

		// These values are optional, and 0 is not a valid index to be directly imported
		textIndex = 0;
		normIndex = 0;

		if (vertexTokens.size() >= 2)
		{
			// There is an optional texture coord index
			textIndex = std::atoi(vertexTokens[1].c_str());
		}		
		if (vertexTokens.size() == 3)
		{
			// There is a normal index
			normIndex = std::atoi(vertexTokens[2].c_str());
		}

		// The relative indices work for whether the index is positive or negative.
		// Positive indices indicate from the start of the list, negative indices indicate from the end of the list

		// Add the vertex index
		newFace->m_Vertices.push_back(
			(vertIndex > 0) ? vertIndex - 1 : m_Model->vertexCount() + vertIndex
		);

		// Add the optional texture coord index
		if (textIndex != 0)
			newFace->m_TextureCoords.push_back(
				(textIndex > 0) ? textIndex - 1 : m_Model->textureCoordCount() + textIndex
			);

		// Add the optional normal index
		if (normIndex != 0)
		{
			newFace->m_Normals.push_back(
				(normIndex > 0) ? normIndex - 1 : m_Model->normalCount() + normIndex
			);
			hasNormal = true;
		}
	}


	if (newFace->m_Vertices.empty())
	{
		GRAV_LOG_LINE_ERROR("OBJ: Imported empty face");
		return;
	}

	//// Set the material if there is one
	//if (m_Model->m_CurrentMaterial != nullptr)
	//	newFace->m_Material = m_Model->m_CurrentMaterial;
	//else
	//	newFace->m_Material = m_Model->m_DefaultMaterial;

	// Create the object if there is not one
	if (m_Model->m_CurrentObject == nullptr)
		createObject(s_DefaultObjectName);

	// Assign the face to a new mesh if the current mesh doesn't exist
	if (m_Model->m_CurrentMesh == nullptr)
		createMesh(s_DefaultObjectName);

	// Store the face in the mesh
	m_Model->m_CurrentMesh->m_Faces.push_back(newFace);


	// Increase the vertex and texture count
	m_Model->m_CurrentMesh->m_VertexCount += (vertexIndex) newFace->m_Vertices.size();
	m_Model->m_CurrentMesh->m_TextureCount += (textureIndex) newFace->m_TextureCoords.size();
	if (m_Model->m_CurrentMesh->m_HasNormals == false && hasNormal) {
		m_Model->m_CurrentMesh->m_HasNormals = true;
	}
}

GRAVEngine::uint8 GRAVEngine::Tools::Importing::objParser::getTextureCoordinate(std::vector<vec2>& textArray, tokenArray& tokens)
{
	uint8 textDim = tokens.size();
	vec3 textureCoordinate;

	if (textDim == 2)
	{
		// Get a 2d texture coordinate
		textureCoordinate = {
			std::atof(tokens[0].c_str()), 
			std::atof(tokens[1].c_str()), 
			0
		};
	}
	else if (textDim == 3)
	{
		// Get a 3d texture coordinate
		textureCoordinate = { 
			std::atof(tokens[0].c_str()),
			std::atof(tokens[1].c_str()),
			std::atof(tokens[2].c_str())
		};
	}
	else
	{
		// Texture coordinates must be dimension 2 or 3
		throw Exceptions::importException("OBJ: Inavalid texture coordinate dimension");
	}

	// Add the texture coordinate
	textArray.push_back(textureCoordinate);

	return textDim;
}

void GRAVEngine::Tools::Importing::objParser::getGroupName(tokenArray& tokens)
{
	std::string groupName = tokens[0];

	// Change the active group if needed
	if (m_Model->m_ActiveGroup != groupName)
	{
		// Search for the group
		auto it = m_Model->m_Groups.find(groupName);

		// In this parser, a group is another object, but the indicies are a part of the new group
		createObject(groupName);

		// If it is a new group, create a new entry
		if (it == m_Model->m_Groups.end())
		{
			// Create a new group if it wasn't found
			ref<std::vector<uint32>> faceIDArray = createRef<std::vector<uint32>>();
			m_Model->m_Groups[groupName] = faceIDArray;
			m_Model->m_GroupFaceIds = faceIDArray;
		}
		else
			m_Model->m_GroupFaceIds = (*it).second;

		// This group is now the active group
		m_Model->m_ActiveGroup = groupName;
	}

}

void GRAVEngine::Tools::Importing::objParser::getGroupNumber(tokenArray& tokens)
{
	// Not used
}

void GRAVEngine::Tools::Importing::objParser::getObjectName(tokenArray& tokens)
{
	std::string objectName = tokens[0];
	if (objectName.empty() == false)
	{
		// Reset the currently pointed object
		m_Model->m_CurrentObject.reset();

		// Check to see if the object exists
		for (auto it = m_Model->m_Objects.begin(); it != m_Model->m_Objects.end(); it++)
		{
			if ((*it)->m_Name == objectName)
			{
				m_Model->m_CurrentObject = *it;
				break;
			}
		}

		if (m_Model->m_CurrentObject == nullptr)
			createObject(objectName);
	}
}
void GRAVEngine::Tools::Importing::objParser::createObject(const std::string& objectName)
{
	// Make sure the model exists
	GRAV_ASSERT(m_Model != nullptr);

	// Create a new object
	m_Model->m_CurrentObject = createRef<objObject>();
	m_Model->m_CurrentObject->m_Name = objectName;
	m_Model->m_Objects.push_back(m_Model->m_CurrentObject);

	// Createa mesh for the object
	createMesh(objectName);

	//if (m_Model->m_CurrentMaterial)
	//{
	//	m_Model->m_CurrentMesh
	//}
}

void GRAVEngine::Tools::Importing::objParser::createMesh(const std::string& meshName)
{
	GRAV_ASSERT(m_Model != nullptr);

	// Create a new mesh
	m_Model->m_CurrentMesh = createRef<objMesh>(meshName);
	m_Model->m_Meshes.push_back(m_Model->m_CurrentMesh);

	// Get the id of the mesh
	meshIndex meshId = m_Model->meshCount() - 1;

	// Attach the mesh to the object
	if (m_Model->m_CurrentObject != nullptr)
		m_Model->m_CurrentObject->m_Meshes.push_back(meshId);
	else
		GRAV_LOG_LINE_ERROR("OBJ: No object to attach mesh {%s} to.", meshName.c_str());
}
//void GRAVEngine::Tools::Importing::objParser::needsMesh(const std::string&)
