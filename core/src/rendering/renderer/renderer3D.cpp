#include "gravpch.h"
#include "renderer3D.h"

#include "rendering/vertexArray.h"
#include "rendering/shaders/shader.h"
#include "rendering/shaders/shaderDataType.h"
#include "rendering/renderer/rendererCommand.h"

#include <glm/gtc/matrix_transform.hpp>


// A vertex of a quad
struct quadVertex
{
	glm::vec3 Position;
	glm::vec4 Color;
	glm::vec2 TexCoord;
	float TexIndex;
	float TilingFactor;
};

// Data used for the renderer2D
struct renderer3DData
{
	// Constants
	static const GRAVEngine::uint32 s_MaxTriangles = 40000;
	static const GRAVEngine::uint32 s_MaxVertices = s_MaxTriangles * 3;
	static const GRAVEngine::uint32 s_MaxIndices = s_MaxTriangles * 3;
	static const GRAVEngine::uint32 s_MaxTextureSlots = 32; // TODO: RenderCaps

	GRAVEngine::ref<GRAVEngine::Rendering::vertexArray>		m_TriangleVertexArray;	// The triangle vertex array
	GRAVEngine::ref<GRAVEngine::Rendering::vertexBuffer>	m_TriangleVertexBuffer;	// The triangle vertex buffer
	GRAVEngine::ref<GRAVEngine::Rendering::shader>			m_TextureShader;	// The rendering texture shader
	GRAVEngine::ref<GRAVEngine::Rendering::texture2D>		m_WhiteTexture;		// The white default texture

	GRAVEngine::uint32 m_TriangleIndexCount = 0;		// The current quad count
	quadVertex* m_TriangleVertexBufferBase = nullptr;	// Array of triangle vertexes
	quadVertex* m_TriangleVertexBufferPtr = nullptr;	// The current triangle vertex

	std::array<GRAVEngine::ref<GRAVEngine::Rendering::texture2D>, s_MaxTextureSlots> m_TextureSlots;	// Array of textures
	GRAVEngine::uint32 m_TextureSlotIndex = 1; // 0 = white texture

	GRAVEngine::Rendering::renderer3D::statistics m_Stats;	// Stats for drawing quads
};

// TODO: Determine if this truly needs to be static and free floating here. Should probably be removed
static renderer3DData s_Data;

void GRAVEngine::Rendering::renderer3D::startup()
{
	GRAV_PROFILE_FUNCTION();

	// Create the data VAO
	s_Data.m_TriangleVertexArray = rendererAPI::getInstance().createVertexArray();

	// Create the vertex buffer for the vertex data
	s_Data.m_TriangleVertexBuffer = rendererAPI::getInstance().createVertexBuffer(s_Data.s_MaxVertices * sizeof(quadVertex));
	s_Data.m_TriangleVertexBuffer->setLayout({
		{shaderDataType::FLOAT3, "a_Position"},
		{shaderDataType::FLOAT4, "a_Color"},
		{shaderDataType::FLOAT2, "a_TexCoord"},
		{shaderDataType::FLOAT, "a_TexIndex"},
		{shaderDataType::FLOAT, "a_TilingFactor"}
		});
	// Add this buffer
	s_Data.m_TriangleVertexArray->addVertexBuffer(s_Data.m_TriangleVertexBuffer);

	// Get the base pointer for the quad vertices
	s_Data.m_TriangleVertexBufferBase = new quadVertex[s_Data.s_MaxVertices];

	// Get the base index for the quad indices
	uint32* triangleIndices = new uint32[s_Data.s_MaxIndices];

	// Create all the possible indices for all of the triangles. This assumes every triangle will be counter clock wise
	uint32 offset = 0;
	for (uint32 i = 0; i < s_Data.s_MaxIndices; i += 3)
	{
		triangleIndices[i + 0] = offset + 0;
		triangleIndices[i + 1] = offset + 1;
		triangleIndices[i + 2] = offset + 2;

		offset += 3;
	}

	// Create the index buffer for the triangles. Can delete the temporary array since the data is copied to the GPU
	ref<indexBuffer> quadIB = rendererAPI::getInstance().createIndexBuffer(triangleIndices, s_Data.s_MaxIndices);
	s_Data.m_TriangleVertexArray->setIndexBuffer(quadIB);
	delete[] triangleIndices;

	// Create the white texture and set its data
	s_Data.m_WhiteTexture = rendererAPI::getInstance().createTexture(1, 1);
	uint32 whiteTextureData = 0xffffffff;
	s_Data.m_WhiteTexture->setData(&whiteTextureData, sizeof(uint32));

	// Set the sampler indices
	int32 samplers[s_Data.s_MaxTextureSlots];
	for (uint32 i = 0; i < s_Data.s_MaxTextureSlots; i++)
		samplers[i] = i;

	// Create the textured shader
	s_Data.m_TextureShader = rendererAPI::getInstance().createShader("assets/shaders/texture.glsl");
	s_Data.m_TextureShader->bind();
	s_Data.m_TextureShader->setIntArray("u_Textures", samplers, s_Data.s_MaxTextureSlots);

	// Set first texture slot to 0
	s_Data.m_TextureSlots[0] = s_Data.m_WhiteTexture;
}
void GRAVEngine::Rendering::renderer3D::shutdown()
{
	delete[] s_Data.m_TriangleVertexBufferBase;
}

void GRAVEngine::Rendering::renderer3D::beginScene(const camera& camera)
{
	GRAV_PROFILE_FUNCTION();
	
	// Set the view matrix
	s_Data.m_TextureShader->bind();
	s_Data.m_TextureShader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());

	startBatch();
}
void GRAVEngine::Rendering::renderer3D::endScene()
{
	GRAV_PROFILE_FUNCTION();

	// Flush the scene for rendering
	flush();
}
void GRAVEngine::Rendering::renderer3D::flush()
{
	GRAV_PROFILE_FUNCTION();

	if (s_Data.m_TriangleIndexCount == 0)
		return; // Nothing to draw

	// Get the size of the data
	uint32 dataSize = (uint32)((uint8*)s_Data.m_TriangleVertexBufferPtr - (uint8*)s_Data.m_TriangleVertexBufferBase);
	s_Data.m_TriangleVertexBuffer->setData(s_Data.m_TriangleVertexBufferBase, dataSize);

	// Bind the textures
	for (uint32 i = 0; i < s_Data.m_TextureSlotIndex; i++)
		s_Data.m_TextureSlots[i]->bind(i);

	// Draw the quads
	s_Data.m_TriangleVertexArray->bind();
	s_Data.m_TextureShader->bind();

	rendererCommand::drawIndexed(s_Data.m_TriangleVertexArray, s_Data.m_TriangleIndexCount);
	s_Data.m_Stats.m_DrawCalls++;
}


void GRAVEngine::Rendering::renderer3D::drawMesh(const vec3& position, const ref<mesh>& mesh, const RGBA& color)
{
	GRAV_PROFILE_FUNCTION();

	glm::mat4 transform = glm::translate(glm::mat4(1.0), position) * glm::scale(glm::mat4(1.0f), { 1.0f, 1.0f, 1.0f });

	drawMesh(transform, mesh, color);

}
void GRAVEngine::Rendering::renderer3D::drawMesh(const mat4x4& transform, const ref<mesh>& mesh, const RGBA& color)
{
	GRAV_PROFILE_FUNCTION();

	const float textureIndex = 0.0f; // White texture
	constexpr glm::vec2 textureCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}};
	const float tilingFactor = 1.0f;

	drawMesh(transform, mesh, s_Data.m_TextureSlots[0], 1.0f, color);
}
void GRAVEngine::Rendering::renderer3D::drawMesh(const mat4x4& transform, const ref<mesh>& mesh, const ref<texture2D>& texture, float tilingFactor, const RGBA& tintColor)
{
	GRAV_PROFILE_FUNCTION();

	constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f } };	// Used if there are not any texture coords

	// See if another batch needs to be made because the max quad count has been reached
	if (s_Data.m_TriangleIndexCount >= renderer3DData::s_MaxIndices)
		nextBatch();

	// Set the texture index values
	float textureIndex = 0.0f;
	for (uint32_t i = 1; i < s_Data.m_TextureSlotIndex; i++)
	{
		if (*s_Data.m_TextureSlots[i] == *texture)
		{
			textureIndex = (float)i;
			break;
		}
	}

	// If the texture index is null
	if (textureIndex == 0.0f)
	{
		// Start a new batch because there are no more texture slots available
		if (s_Data.m_TextureSlotIndex >= renderer3DData::s_MaxTextureSlots)
			nextBatch();

		textureIndex = (float)s_Data.m_TextureSlotIndex;
		s_Data.m_TextureSlots[s_Data.m_TextureSlotIndex] = texture;
		s_Data.m_TextureSlotIndex++;
	}

	// For each face, add the vertex information
	for (size_t i = 0; i < mesh->faceCount(); i++)
	//for (size_t i = 0; i < 6; i++)
	{
		// Check if this face needs to start a new batch because of reaching the limit
		if (s_Data.m_TriangleIndexCount + mesh->m_Faces[i].m_VertexCount >= renderer3DData::s_MaxIndices)
			nextBatch();

		// For each vertex in the face
		for (size_t j = 0; j < mesh->m_Faces[i].m_VertexCount; j++)
		{
			// Get the vertex index
			vertexIndex vert = mesh->m_Faces[i].m_Vertices[j];
			vec3 position = mesh->m_Vertices[vert];

			// Set its data
			s_Data.m_TriangleVertexBufferPtr->Position = transform * vec4(position, 1.0f);
			s_Data.m_TriangleVertexBufferPtr->Color = tintColor;
			if (mesh->hasTextureCoords())
				s_Data.m_TriangleVertexBufferPtr->TexCoord = mesh->m_TextureCoords[vert];
			else
				s_Data.m_TriangleVertexBufferPtr->TexCoord = textureCoords[j % 3];

			s_Data.m_TriangleVertexBufferPtr->TexIndex = textureIndex;
			s_Data.m_TriangleVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.m_TriangleVertexBufferPtr++;
		}

		// Increase the index count based on the number of face vertices
		s_Data.m_TriangleIndexCount += mesh->m_Faces[i].m_VertexCount;
		s_Data.m_Stats.m_TriangleCount++;
	}

}

void GRAVEngine::Rendering::renderer3D::resetStats()
{
	memset(&s_Data.m_Stats, 0, sizeof(statistics));
}
GRAVEngine::Rendering::renderer3D::statistics GRAVEngine::Rendering::renderer3D::getStats()
{
	return s_Data.m_Stats;
}

void GRAVEngine::Rendering::renderer3D::startBatch()
{
	// Reset the quad count and quad pointer
	s_Data.m_TriangleIndexCount = 0;
	s_Data.m_TriangleVertexBufferPtr = s_Data.m_TriangleVertexBufferBase;

	// Start at the default texture again
	s_Data.m_TextureSlotIndex = 1;

}
void GRAVEngine::Rendering::renderer3D::nextBatch()
{
	// Flush the current batch and start a new one
	flush();
	startBatch();
}
