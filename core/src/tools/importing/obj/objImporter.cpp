#include "gravpch.h"
#include "objImporter.h"
#include "io/io.h"
#include "io/file.h"
#include "exceptions/importing/importException.h"
#include "objParser.h"

static const GRAVEngine::Tools::Importing::importerDescription s_Desc = {
    "Wavefront Object Importer",
    "surfaces not supported",
    0,      //aiImporterFlags_SupportTextFlavour,
    0,      // Min Version Major
    0,      // Min Version Minor
    0,      // Max Version Major
    0      // Max Version Minor
};
static const size_t s_MinOBJFileSize = 16;

GRAVEngine::Tools::Importing::objImporter::objImporter()
{
    m_FileExtensions = std::set < std::string>();
    m_FileExtensions.insert(".obj");
}

bool GRAVEngine::Tools::Importing::objImporter::canRead(const std::string& path, bool checkSig) const
{
    if (checkSig)
        return isExtentensionSupported(IO::fileExtension(path));
    else
    {
        // Check the file header for tokens

        IO::textFile file = IO::textFile(path, IO::fileMode::INPUT, false);
        const std::string tokens[] = { "mtllib", "usemtl", "v ", "vt ", "vn ", "o ", "g ", "s ", "f " };

        // Search the file for the tokens
        return file.searchForToken(tokens, 9, 200);
    }
}

void GRAVEngine::Tools::Importing::objImporter::internalReadFile(const std::string& path, scope<scene>& scene)
{
    // Open the file
    IO::textFile file = IO::textFile(path, IO::fileMode::INPUT, false);

    // Check the file size of the obj file
    if (file.fileSize() < s_MinOBJFileSize)
        throw Exceptions::importException("OBJ-file is too small.");

    // Get the model and folder name
    std::string modelName = IO::pathStem(path);
    std::string folderName = IO::pathStem(IO::parentPath(path));

    // Buffer for the parser
    std::vector<char> buffer;

    // Parse the file
    objParser parser(file, modelName, folderName);

    // Create a scene from the parsed file
    createDataFromImport(parser.getModel(), scene);
}

void GRAVEngine::Tools::Importing::objImporter::setupProperties(const ref<importer>& importer)
{
}

const GRAVEngine::Tools::Importing::importerDescription GRAVEngine::Tools::Importing::objImporter::getDescription() const
{
    return s_Desc;
}

void GRAVEngine::Tools::Importing::objImporter::createDataFromImport(const ref<objModel>& model, scope<scene>& scene)
{
    // Can't import anything if the model doesn't exist
    if (model == nullptr)
        return;

    scene->m_RootNode = createScope<node>();
    if (model->m_ModelName.empty() == false)
        scene->m_RootNode->m_Name = model->m_ModelName;
    else
        throw Exceptions::importException("Model does not have name");

    if (model->m_Objects.empty() == false)
    {
        meshIndex meshCount = 0;
        meshIndex childCount = 0;

        // Determine how many child objects there are
        for (auto object : model->m_Objects)
        {
            if (object)
            {
                childCount++;
                meshCount += (meshIndex) object->m_Meshes.size();
            }
        }

        // Allocate space for the child nodes on the root node
        scene->m_RootNode->m_Children = createScope<ref<node>[]>(childCount);

        // Create nodes for the scene
        std::vector<ref<mesh>> meshes;
        meshes.reserve(meshCount);
        for (size_t i = 0; i < model->objectCount(); i++)
            createNode(model, model->m_Objects[i], scene->m_RootNode, scene, meshes);

        // Make sure all of the children were made
        GRAV_ASSERT(scene->m_RootNode->m_ChildCount == childCount);

        // Create mesh pointer buffer for this cene
        if (scene->m_MeshCount > 0)
        {
            scene->m_Meshes = createScope<ref<mesh>[]>(meshes.size());
            for (size_t i = 0; i < meshes.size(); i++)
                scene->m_Meshes[i] = meshes[i];
        }

        // Create the materials
    }
    else
    {
        // Not a single object was referenced in the file

        // No Vertices read
        if (model->vertexCount() == 0)
            return;

        // Create a single mesh for all of the vertices
        ref<mesh> newMesh = createRef<mesh>();
        newMesh->m_PrimitiveTypes = (uint32) primitiveType::POINT;
        newMesh->m_VertexCount = model->vertexCount();

        // Create the vertex array on the mesh and copy the data
        newMesh->m_Vertices = createScope<vec3[]>(newMesh->m_VertexCount);
        memcpy(&newMesh->m_Vertices, model->m_Vertices.data(), newMesh->m_VertexCount * sizeof(vec3));

        // Copy the normals
        if (model->normalCount() > 0)
        {
            newMesh->m_Normals = createScope<vec3[]>(newMesh->vertexCount());
            if (model->normalCount() < newMesh->vertexCount())
                throw Exceptions::importException("OBJ: Too few normals.");
            memcpy(&newMesh->m_Normals, model->m_Normals.data(), newMesh->vertexCount() * sizeof(vec3));
        }

        // Copy the colors
        if (model->colorCount() > 0)
        {
            newMesh->m_Colors = createScope<RGBA[]>(newMesh->vertexCount());
            if (model->colorCount() < newMesh->vertexCount())
                throw Exceptions::importException("OBJ: Too few colors.");
            memcpy(&newMesh->m_Colors, model->m_TextCoords.data(), newMesh->vertexCount() * sizeof(RGBA));
        }

        // Create the root nodes mesh array
        scene->m_RootNode->m_MeshCount = 1;
        scene->m_RootNode->m_Meshes = createScope<meshIndex[]>(1);
        scene->m_RootNode->m_Meshes[0] = 0;

        // Create the scene's single mesh
        scene->m_Meshes = createScope<ref<mesh>[]>(1);
        scene->m_MeshCount = 1;
        scene->m_Meshes[0] = newMesh;
    }
}

GRAVEngine::ref<GRAVEngine::Tools::Importing::node> GRAVEngine::Tools::Importing::objImporter::createNode(const ref<objModel>& model, const ref<objObject>& object, ref<node>& parent, scope<scene>& scene, std::vector<ref<mesh>>& meshArray)
{
    GRAV_ASSERT(model != nullptr);
    if (object == nullptr)
        return nullptr;

    // Store older mesh
    size_t oldMeshSize = meshArray.size();
    ref<node> newNode = createRef<node>(object->m_Name);

    // Store the parent node if there is one and set the parent
    newNode->m_Parent = parent;
    parent->m_ChildCount++;
    parent->m_Children[parent->m_ChildCount - 1] = newNode;


    for (auto it = object->m_Meshes.begin(); it != object->m_Meshes.end(); it++)
    {
        ref<mesh> mesh = createTopology(model, object, *it);

        // Add the mesh if it was created and has at least 1 face
        if (mesh && mesh->faceCount() > 0)
            meshArray.push_back(mesh);
    }

    //// Create all nodes for subobjects
    //if (object->hasSubobjects() == false)
    //{
    //    // Create the children array
    //    newNode->m_ChildCount = object->subObjectCount();
    //    newNode->m_Children = createScope<ref<node>[]>(object->subObjectCount());

    //    //newNode->m_MeshCount = 1;
    //    //newNode->m_Meshes = createScope<meshIndex[]>(1);
    //}

    // Set mesh instances into scene and note instances
    size_t meshSizeDiff = meshArray.size() - oldMeshSize;
    if (meshSizeDiff > 0)
    {
        // Create the mesh array for the node
        newNode->m_Meshes = createScope<meshIndex[]>(meshSizeDiff);
        newNode->m_MeshCount = (meshIndex) meshSizeDiff;

        // For every new mesh, increase the mesh count and the mesh indices
        size_t index = 0;
        for (size_t i = oldMeshSize; i < meshArray.size(); i++)
        {
            newNode->m_Meshes[index] = scene->m_MeshCount;
            scene->m_MeshCount++;
            ++index;
        }
    }

    return newNode;
}

GRAVEngine::ref<GRAVEngine::mesh> GRAVEngine::Tools::Importing::objImporter::createTopology(const ref<objModel>& model, const ref<objObject>& object, meshIndex meshIndex)
{
    GRAV_ASSERT(model != nullptr);
    
    // Can't make topology without an object
    if (object == nullptr)
        return nullptr;

    // Get the mesh data
    ref<objMesh> meshData = model->m_Meshes[meshIndex];

    // No mesh created if there is no data
    if (meshData == nullptr)
        return nullptr;

    // No mesh created if there is no data
    if (meshData->m_Faces.empty())
        return nullptr;

    // Create the new mesh
    ref<mesh> newMesh = createRef<mesh>();
    if (meshData->m_Name.empty() == false)
        newMesh->m_Name = meshData->m_Name;

    // For each face, get the primitive type and the number of faces for each primitive type
    for (auto it = meshData->m_Faces.begin(); it != meshData->m_Faces.end(); it++)
    {
        // Make sure there is actually face data
        GRAV_ASSERT((*it) != nullptr);

        if ((*it)->m_PrimitiveType == primitiveType::LINE)
        {
            // A line segment will always have 1 less the amount of vertices faces
            newMesh->m_FaceCount += (*it)->vertexCount() - 1;
            newMesh->m_PrimitiveTypes |= (uint32) primitiveType::LINE;
        }
        else if ((*it)->m_PrimitiveType == primitiveType::POINT)
        {
            // Each point is its own face
            newMesh->m_FaceCount += (*it)->vertexCount();
            newMesh->m_PrimitiveTypes |= (uint32) primitiveType::POINT;
        }
        else
        {
            // Anything greather than a line is a face notmatter the amount of vertices
            newMesh->m_FaceCount++;
            if ((*it)->vertexCount() >= 5)
                newMesh->m_PrimitiveTypes |= (uint32)primitiveType::NGON;
            else if ((*it)->vertexCount() == 4)
                newMesh->m_PrimitiveTypes |= (uint32)primitiveType::QUAD;
            else if ((*it)->vertexCount() == 3)
                newMesh->m_PrimitiveTypes |= (uint32)primitiveType::TRIANGLE;
        }
    }

    vertexIndex vertCount = 0;
    if (newMesh->faceCount() > 0)
    {
        // Create the face array
        newMesh->m_Faces = createScope<face[]>(newMesh->faceCount());

        //if (meshData->m_MaterialIndex != mesh::NoMaterial) {
        //    Mesh->mMaterialIndex = pObjMesh->m_uiMaterialIndex;
        //}

        // Copy the face infomation from the meshData and store it in the newly created mesh
        uint32 faceIndex = 0;
        for (auto it = meshData->m_Faces.begin(); it != meshData->m_Faces.end(); it++)
        {
            if ((*it)->m_PrimitiveType == primitiveType::LINE)
            {
                // If the face is a line
                for (size_t i = 0; i < (*it)->vertexCount() - 1; i++)
                {
                    // For every pair of vertices (A single line segment)
                    face* faceData = &newMesh->m_Faces[faceIndex++];        // Get the face
                    faceData->m_VertexCount = 2;                            // Set the vertex count of the face
                    vertCount += faceData->m_VertexCount;                   // Increase the total vertex count
                    faceData->m_Vertices = createScope<vertexIndex[]>(faceData->m_VertexCount);
                }
            }
            else if ((*it)->m_PrimitiveType == primitiveType::POINT)
            {
                // If it is a point. Every vertex is a face
                for (size_t i = 0; i < (*it)->vertexCount(); i++)
                {
                    face* faceData = &newMesh->m_Faces[faceIndex++];    // Get the face
                    faceData->m_VertexCount = 1;                        // Set the vertex count of the face
                    vertCount += faceData->m_VertexCount;               // Increase the total vertex count
                    faceData->m_Vertices = createScope<vertexIndex[]>(faceData->m_VertexCount);
                }
            }
            else
            {
                // If it is an ngon
                face* faceData = &newMesh->m_Faces[faceIndex++];    // Get the face
                faceData->m_VertexCount = (*it)->vertexCount();     // Set the vertex count of the face
                vertCount += faceData->m_VertexCount;               // Increase the total vertex count

                // Only make a face if there was actually at least 1 vertex. This isn't done for the other primitive types because this check is implicit in the for loop
                if (faceData->m_VertexCount > 0)
                    faceData->m_Vertices = createScope<vertexIndex[]>(faceData->m_VertexCount);

            }
        }
    }

    // Create the mesh vertices
    createVertexArray(model, object, meshIndex, newMesh, vertCount);


    return newMesh;
}

void GRAVEngine::Tools::Importing::objImporter::createVertexArray(const ref<objModel>& model, const ref<objObject>& object, meshIndex meshIndex, ref<mesh>& newMesh, vertexIndex vertCount)
{
    // TODO: Simplify this function

    GRAV_ASSERT(object != nullptr);

    // Don't create any information if the mesh is empty
    if (object->m_Meshes.empty())
        return;

    // Get the current mesh
    ref<objMesh> meshData = model->m_Meshes[meshIndex];
    // No processing if there are no meshes or vertices
    if (meshData == nullptr || meshData->m_VertexCount == 0)
        return;

    // Copy vertices of this mesh instance
    newMesh->m_VertexCount = vertCount;
    if (newMesh->m_VertexCount == 0)
        throw Exceptions::importException("OBJ: No Vertices");
    else if (newMesh->m_VertexCount > GRAV_MAX_VERTICES)
        throw Exceptions::importException("OBJ: Too many vertices");

    // Create the vertex array
    newMesh->m_Vertices = createScope<vec3[]>(newMesh->m_VertexCount);

    // Allocate for normals
    if (meshData->m_HasNormals && model->normalCount() > 0)
        newMesh->m_Normals = createScope<vec3[]>(newMesh->vertexCount());
    // Allocate for colors
    if (meshData->m_HasVertexColors && model->colorCount() > 0)
        newMesh->m_Colors = createScope<RGBA[]>(newMesh->vertexCount());
    // Allocate for texture coordinates
    if (model->textureCoordCount() > 0)
        newMesh->m_TextureCoords = createScope<vec2[]>(newMesh->vertexCount());

    bool hasMoreNormals = true;
    bool hasMoreUVs = true;
    vertexIndex newIndex = 0;
    vertexIndex outputFaceIndex = 0;

    // Copy vertices, normals, and textures
    for (auto it = meshData->m_Faces.begin(); it != meshData->m_Faces.end(); it++)
    {
        // Copy all the index arrays

        // For every vertex
        for (size_t buffIndex = 0, outputFaceVertexIndex = 0; buffIndex < (*it)->vertexCount(); buffIndex++)
        {
            // vertIndex is the current vertex

            // Get the vertex index
            vertexIndex vertex = (*it)->m_Vertices[buffIndex];

            if (vertex >= model->vertexCount())
                throw Exceptions::importException("OBJ: vertex index out of range");
            if (newMesh->vertexCount() <= newIndex)
                throw Exceptions::importException("OBJ: bad vertex index");

            // Copy the vertices from the model
            newMesh->m_Vertices[newIndex] = model->m_Vertices[vertex];


            // Copy the normals
            if (hasMoreNormals && model->normalCount() > 0 && buffIndex < (*it)->normalCount())
            {
                normalIndex normal = (*it)->m_Normals[buffIndex];
                if (normal >= model->normalCount())
                    hasMoreNormals = false;
                else
                    newMesh->m_Normals[newIndex] = model->m_Normals[normal];
            }

            // Copy the vertex colors
            if (model->colorCount() > 0)
            {
                vec3 color = model->m_VertexColors[vertex];
                newMesh->m_Colors[newIndex] = { color.x, color.y, color.z, 1.0f };
            }

            // Copy the texture coordinates
            if (hasMoreUVs && model->textureCoordCount() > 0 && buffIndex < (*it)->textureCoordCount())
            {
                textureIndex texture = (*it)->m_TextureCoords[buffIndex];

                if (texture >= model->textureCoordCount())
                    hasMoreUVs = false;
                else
                    newMesh->m_TextureCoords[newIndex] = model->m_TextCoords[texture];
            }


            // Get the new vertex index relative to the mesh and face
            face* destinationFace = &newMesh->m_Faces[outputFaceIndex];

            bool isLast = (buffIndex == (*it)->vertexCount() - 1);  // Is the current buffer index the last index
            if ((*it)->m_PrimitiveType != primitiveType::LINE || isLast == false)
            {
                // If it is not a line (Because segments) and is not the last vertex set its vertex
                destinationFace->m_Vertices[outputFaceVertexIndex] = newIndex;
                outputFaceVertexIndex++;  // The next vertex in the face will be next value
            }


            if ((*it)->m_PrimitiveType == primitiveType::POINT)
            {
                outputFaceIndex++;          // The next face
                outputFaceVertexIndex = 0;  // Points will always start from 0
            }
            else if ((*it)->m_PrimitiveType == primitiveType::LINE)
            {
                outputFaceVertexIndex = 0;  // Lines will always start from 0

                if (isLast == false)
                    outputFaceIndex++;      // The output facw is the next face

                if (buffIndex)
                {
                    // If it is not the first index
                    if (isLast == false)
                    {
                        // And is not the last
                        newMesh->m_Vertices[(size_t) newIndex + 1] = newMesh->m_Vertices[newIndex];  // Get for pairs

                        // The normals are also paired
                        if ((*it)->vertexCount() > 0 && model->normalCount() > 0)
                            newMesh->m_Normals[(size_t) newIndex + 1] = newMesh->m_Normals[newIndex];
                        // The texture coords are also paired
                        if (model->textureCoordCount() > 0)
                            newMesh->m_TextureCoords[(size_t) newIndex + 1] = newMesh->m_TextureCoords[newIndex];

                        newIndex++; // Get the next vertex. This is extra because lines come in pairs
                    }

                    // The previous face's first vertex is the new index
                    destinationFace[-1].m_Vertices[1] = newIndex;
                }
            }
            else if (isLast)
                outputFaceIndex++;  // Get the next face


            newIndex++; // Get the next vertex
        }
    }

    // Remove the reference to the normals as there to many normals
    if (hasMoreNormals == false)
        newMesh->m_Normals = nullptr;

    // Remove the reference to the uvs as there are too many uvs
    if (hasMoreUVs == false)
        newMesh->m_TextureCoords = nullptr;
}


