#pragma once

#include "common.h"
#include "types/mats/mat4x4.h"
#include "types/geometry.h"
#include <string>
#include <vector>

namespace GRAVEngine
{
	namespace Tools
	{
		namespace Importing
		{
			// OBJ object
			struct objObject
			{
			public:
				std::string m_Name;
				mat4x4 m_Transformation;
				//std::vector<ref<objObject[]>> m_SubObjects;

				std::vector<meshIndex> m_Meshes;

			public:
				//inline const bool hasSubobjects() const { return m_SubObjects.size() > 0; }

				//inline const size_t subObjectCount() const { return m_SubObjects.size(); }
			};

			// OBJ face
			struct objFace
			{
			public:
				primitiveType m_PrimitiveType;				// Face primitive type

				std::vector<vertexIndex> m_Vertices;		// Vertex indices
				std::vector<normalIndex> m_Normals;			// Normal indices
				std::vector<textureIndex> m_TextureCoords;	// Texture coordinate indices

				//ref<material> m_Material;

			public:
				objFace(primitiveType primitiveType = primitiveType::NGON) : m_PrimitiveType(primitiveType), m_Vertices(), m_Normals(), m_TextureCoords() {}
				~objFace() {}

				inline const vertexIndex vertexCount() const { return (vertexIndex) m_Vertices.size(); }
				inline const normalIndex normalCount() const { return (normalIndex) m_Normals.size(); }
				inline const textureIndex textureCoordCount() const { return (textureIndex) m_TextureCoords.size(); }
			};

			// OBJ Mesh
			struct objMesh
			{
			public:
				std::string m_Name;					// Name of the mesh
				std::vector<ref<objFace>> m_Faces;	// All of the faces
				//ref<objMaterial> m_Material;		// Mesh Material

				vertexIndex m_VertexCount;
				textureIndex m_TextureCount;
				//materialIndex m_MaterialIndex;

				bool m_HasNormals;
				bool m_HasVertexColors;

			public:
				objMesh(const std::string& name) :
					m_Name(name), m_Faces(), /*m_Material(nullptr),*/ m_VertexCount(0), m_TextureCount(0), /*m_MaterialIndex(0),*/ m_HasNormals(false), m_HasVertexColors(false)
				{}
				~objMesh() {}


			};


			struct objModel
			{
			public:
				std::string m_ModelName;				// Name of the model

				std::vector<ref<objObject>> m_Objects;	// All of the objects in this model
				ref<objObject> m_CurrentObject;			// The current object
				

				//std::vector<std::string> m_Materials;
				//std::unordered_map<std::string, ref<objMaterial>> m_MaterialMap;
				//ref<objMaterial> m_DefaultMaterial;
				//ref<objMaterial> m_CurrentMaterial;

				std::vector<vec3> m_Vertices;		// All the verticies
				std::vector<vec3> m_Normals;		// All the vertex normals
				std::vector<vec3> m_VertexColors;	// All the vertex colors
				std::vector<vec2> m_TextCoords;		// All the texture coordinates

				std::vector<ref<objMesh>> m_Meshes;	// All the meshes
				ref<objMesh> m_CurrentMesh;			// The current mesh

				std::unordered_map<std::string, ref<std::vector<uint32>>> m_Groups;	// Group name to group 
				ref<std::vector<uint32>> m_GroupFaceIds;							// Group with face ids
				std::string m_ActiveGroup;


			public:
				inline const meshIndex meshCount() const { return (meshIndex) m_Meshes.size(); }
				inline const vertexIndex vertexCount() const { return (vertexIndex) m_Vertices.size(); }
				inline const normalIndex normalCount() const { return (normalIndex) m_Normals.size(); }
				inline const vertexIndex colorCount() const { return (vertexIndex) m_VertexColors.size(); }
				inline const textureIndex textureCoordCount() const { return (textureIndex) m_TextCoords.size(); }
				inline const size_t objectCount() const { return m_Objects.size(); }

			public:
				objModel() : m_ModelName(""), m_CurrentObject(nullptr), m_CurrentMesh(nullptr), m_GroupFaceIds(nullptr), m_ActiveGroup("")
				{}

				~objModel() { }

			};
		}
	}
}