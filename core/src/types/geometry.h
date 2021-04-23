#pragma once

#include <vector>
#include <string>
#include "types/vecs/vec2.h"
#include "types/vecs/vec3.h"
#include "types/vecs/vec4.h"
#include "color.h"
#include "primitiveType.h"
#include "dataPrimitives.h"

// Maximum number of face indices
#ifndef GRAV_MAX_FACE_INDICES
#define GRAV_MAX_FACE_INDICES 0x7fff
#endif

// Maximum number of face vertices
#ifndef GRAV_MAX_VERTICES
#define GRAV_MAX_VERTICES 0x7fffffff
#endif

// Maximum number of faces
#ifndef GRAV_MAX_FACES
#define GRAV_MAX_FACES 0x7fffffff
#endif

namespace GRAVEngine
{
	typedef uint32 faceIndex;
	typedef uint32 meshIndex;
	typedef uint32 vertexIndex;
	typedef uint32 normalIndex;
	typedef uint32 textureIndex;

	typedef vec4 plane;

	// A ray
	struct ray
	{
		ray() : m_Pos(), m_Dir() {}
		ray(const vec3& pos, const vec3 dir) : m_Pos(pos), m_Dir(dir) {}
		ray(const ray& other) : m_Pos(other.m_Pos), m_Dir(other.m_Dir) {}
		ray(ray&& other) noexcept : m_Pos(std::move(other.m_Pos)), m_Dir(std::move(other.m_Dir)) {}
		ray& operator=(const ray& other)
		{
			if (this != &other)
			{
				m_Pos = other.m_Pos;
				m_Dir = other.m_Dir;
			}

			return *this;
		}
		ray& operator=(ray&& other) noexcept
		{
			if (this != &other)
			{
				m_Pos = std::move(other.m_Pos);
				m_Dir = std::move(other.m_Dir);
			}

			return *this;
		}

		vec3 m_Pos;	// Ray start position
		vec3 m_Dir;	// Ray direction
	};

	// The face of an object
	struct face
	{
	public:
		scope<vertexIndex[]> m_Vertices;	// Vertex indices
		vertexIndex m_VertexCount;			// Vertex count

		face() : m_Vertices(nullptr), m_VertexCount(0) {}
		face(const face& other)
		{
			// Use the copy assignment
			*this = other;
		}
		~face() {}

		face& operator=(const face& other)
		{
			if (this != &other)
			{
				// Get the vertex count
				m_VertexCount = other.m_VertexCount;

				if (m_VertexCount)
				{
					// Need to make a deep copy of the vertices
					m_Vertices = createScope<vertexIndex[]>(m_VertexCount);

					// Copy the data
					memcpy(m_Vertices.get(), other.m_Vertices.get(), m_VertexCount * sizeof(vertexIndex));
				}
				else
					m_Vertices = nullptr;	// Null array if there are no items
			}

			return *this;
		}

		// Equality Comparison
		bool operator==(const face& other) const
		{
			// Initial check vertex pointers
			if (m_Vertices == other.m_Vertices)
				return true;

			// Check vertex counts
			if (m_VertexCount != other.m_VertexCount)
				return false;

			if (m_Vertices == nullptr || other.m_Vertices == nullptr)
				return false;

			// Compare the memory
			if (memcmp(m_Vertices.get(), other.m_Vertices.get(), m_VertexCount * sizeof(vertexIndex)) != 0)
				return false;

			return true;
		}
		// Not Equality Comparison
		bool operator!=(const face& other) const
		{
			return !(*this == other);
		}

	};

	//struct bone
	//{
	//
	//};
	//struct vertexWeight
	//{
	//
	//};

	struct mesh
	{
	public:
		std::string m_Name;				// name of the mesh

		uint32  m_PrimitiveTypes;		// Bitwise combination of all the types present in the mesh

		vertexIndex m_VertexCount;		// Number of vertices
		scope<vec3[]> m_Vertices;		// Vertices
		scope<vec3[]> m_Normals;		// Vertex Normals
		//scope<vec3[]> m_Tangents;		// Vertex Tangents
		//scope<vec3[]> m_Bitangents;		// Vertex Bitangents
		scope<vec2[]> m_TextureCoords;	// Vertex Texture Coords		
		scope<RGBA[]> m_Colors;			// Vertex Colors

		faceIndex m_FaceCount;			// Number of faces
		scope<face[]> m_Faces;				// Faces

		//boneIndex m_BoneCount;			// Number of bones
		//scope<scope<bone[]>[]> m_Bones;	// Bones

		//materialIndex m_MaterialIndex;	// Material used by this mesh

		mesh() : mesh("") {}
		mesh(const std::string& meshName) : 
			m_Name(meshName), m_PrimitiveTypes((uint32) primitiveType::NONE), 
			m_VertexCount(0), m_Vertices(nullptr), m_Normals(nullptr)/*, m_Tangents(nullptr), m_Bitangents(nullptr)*/, m_TextureCoords(nullptr), m_Colors(nullptr),
			m_FaceCount(0), m_Faces(nullptr)
		{}
		~mesh() {}

		inline const vertexIndex vertexCount() const { return m_VertexCount; }
		inline const faceIndex faceCount() const { return m_FaceCount; }
		//inline const boneIndex boneCount() const { return m_BoneCount; }

		inline const bool hasPositions() const { return m_VertexCount > 0 && m_Vertices != nullptr; }
		inline const bool hasFaces() const { return m_FaceCount > 0 && m_Faces != nullptr; }
		
		inline const bool hasNormals() const { return m_VertexCount > 0 && m_Normals != nullptr; }
		//inline const bool hasTangetsAndBitangents() const { return m_VertexCount > 0 && m_Tangents != nullptr && m_Bitangents != nullptr; }
		inline const bool hasTextureCoords() const { return m_VertexCount > 0 && m_TextureCoords != nullptr; }
		inline const bool hasColors() const { return m_VertexCount > 0 && m_Colors != nullptr; }
	};
}