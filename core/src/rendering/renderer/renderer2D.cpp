#include "gravpch.h"
#include "renderer2D.h"

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
struct renderer2DData
{
	// Constants
	static const GRAVEngine::uint32 s_MaxQuads = 20000;
	static const GRAVEngine::uint32 s_MaxVertices = s_MaxQuads * 4;
	static const GRAVEngine::uint32 s_MaxIndices = s_MaxQuads * 6;
	static const GRAVEngine::uint32 s_MaxTextureSlots = 32; // TODO: RenderCaps

	GRAVEngine::Rendering::vertexArray*		m_QuadVertexArray;	// The quad vertex array
	GRAVEngine::Rendering::vertexBuffer*	m_QuadVertexBuffer;	// The quad vertex buffer
	GRAVEngine::Rendering::shader*			m_TextureShader;	// The rendering texture shader
	GRAVEngine::Rendering::texture2D*		m_WhiteTexture;		// The white default texture

	GRAVEngine::uint32 m_QuadIndexCount	= 0;		// The current quad count
	quadVertex* m_QuadVertexBufferBase	= nullptr;	// Array of quad vertexes
	quadVertex* m_QuadVertexBufferPtr	= nullptr;	// The current quad vertex

	std::array<GRAVEngine::Rendering::texture2D*, s_MaxTextureSlots> m_TextureSlots;	// Array of textures
	GRAVEngine::uint32 m_TextureSlotIndex = 1; // 0 = white texture

	glm::vec4 m_QuadVertexPositions[4];	// The default quad vertex positions

	GRAVEngine::Rendering::renderer2D::statistics m_Stats;	// Stats for drawing quads
};

// TODO: Determine if this truly needs to be static and free floating here. Should probably be removed
static renderer2DData s_Data;

void GRAVEngine::Rendering::renderer2D::startup()
{
	// Create the data VAO
	s_Data.m_QuadVertexArray = rendererAPI::getInstance()->createVertexArray();

	// Create the vertex buffer for the vertex data
	s_Data.m_QuadVertexBuffer = rendererAPI::getInstance()->createVertexBuffer(s_Data.s_MaxVertices * sizeof(quadVertex));
	s_Data.m_QuadVertexBuffer->setLayout({
		{shaderDataType::FLOAT3, "a_Position"},
		{shaderDataType::FLOAT4, "a_Color"},
		{shaderDataType::FLOAT2, "a_TexCoord"},
		{shaderDataType::FLOAT, "a_TexIndex"},
		{shaderDataType::FLOAT, "a_TilingFactor"}
		});
	// Add this buffer
	s_Data.m_QuadVertexArray->addVertexBuffer(s_Data.m_QuadVertexBuffer);

	// Get the base pointer for the quad vertices
	s_Data.m_QuadVertexBufferBase = new quadVertex[s_Data.s_MaxVertices];

	// Get the base index for the quad indices
	uint32* quadIndices = new uint32[s_Data.s_MaxIndices];

	// Create all the possible indices for all of the triangles in the quads
	uint32 offset = 0;
	for (uint32 i = 0; i < s_Data.s_MaxIndices; i += 6)
	{
		quadIndices[i + 0] = offset + 0;
		quadIndices[i + 1] = offset + 1;
		quadIndices[i + 2] = offset + 2;

		quadIndices[i + 3] = offset + 2;
		quadIndices[i + 4] = offset + 3;
		quadIndices[i + 5] = offset + 0;

		offset += 4;
	}

	// Create the index buffer for the quads. Can delete the temporary array since the data is copied to the GPU
	indexBuffer* quadIB= rendererAPI::getInstance()->createIndexBuffer(quadIndices, s_Data.s_MaxIndices);
	s_Data.m_QuadVertexArray->setIndexBuffer(quadIB);
	delete[] quadIndices;

	// Create the white texture and set its data
	s_Data.m_WhiteTexture = rendererAPI::getInstance()->createTexture(1, 1);
	uint32 whiteTextureData = 0xffffffff;
	s_Data.m_WhiteTexture->setData(&whiteTextureData, sizeof(uint32));

	// Set the sampler indices
	int32 samplers[s_Data.s_MaxTextureSlots];
	for (uint32 i = 0; i < s_Data.s_MaxTextureSlots; i++)
		samplers[i] = i;

	// Create the textured shader
	s_Data.m_TextureShader = rendererAPI::getInstance()->createShader("assets/shaders/texture.glsl");
	s_Data.m_TextureShader->bind();
	s_Data.m_TextureShader->setIntArray("u_Textures", samplers, s_Data.s_MaxTextureSlots);

	// Set first texture slot to 0
	s_Data.m_TextureSlots[0] = s_Data.m_WhiteTexture;

	// Set the default quad positions
	s_Data.m_QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
	s_Data.m_QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
	s_Data.m_QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
	s_Data.m_QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
}
void GRAVEngine::Rendering::renderer2D::shutdown()
{
	delete[] s_Data.m_QuadVertexBufferBase;
}

void GRAVEngine::Rendering::renderer2D::beginScene(const camera& camera, const glm::mat4& transform)
{
	// Get the projection of the camera
	glm::mat4 viewProj = camera.getProjectionMatrix() * glm::inverse(transform);

	// Set the view matrix
	s_Data.m_TextureShader->bind();
	s_Data.m_TextureShader->setMat4("u_ViewProjection", viewProj);

	startBatch();
}
void GRAVEngine::Rendering::renderer2D::beginScene(const editorCamera& camera)
{
	// Get the projection of the camera
	glm::mat4 viewProj = camera.getViewProjectionMatrix();

	// Set the view matrix
	s_Data.m_TextureShader->bind();
	s_Data.m_TextureShader->setMat4("u_ViewProjection", viewProj);

	startBatch();
}
void GRAVEngine::Rendering::renderer2D::beginScene(const orthographicCamera& camera)
{
	// Set the texture view project matrix
	s_Data.m_TextureShader->bind();
	s_Data.m_TextureShader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());

	// Start the batch for rendering
	startBatch();
}
void GRAVEngine::Rendering::renderer2D::endScene()
{
	// Flush the scene for rendering
	flush();
}
void GRAVEngine::Rendering::renderer2D::flush()
{
	if (s_Data.m_QuadIndexCount == 0)
		return; // Nothing to draw

	// Get the size of the data
	uint32 dataSize = (uint32)((uint8*)s_Data.m_QuadVertexBufferPtr - (uint8*)s_Data.m_QuadVertexBufferBase);
	s_Data.m_QuadVertexBuffer->setData(s_Data.m_QuadVertexBufferBase, dataSize);

	// Bind the textures
	for (uint32 i = 0; i < s_Data.m_TextureSlotIndex; i++)
		s_Data.m_TextureSlots[i]->bind(i);

	// Draw the quads
	rendererCommand::drawIndexed(s_Data.m_QuadVertexArray, s_Data.m_QuadIndexCount);
	s_Data.m_Stats.m_DrawCalls++;
}

void GRAVEngine::Rendering::renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
	drawQuad({ position.x, position.y, 0.0f }, size, color);
}
void GRAVEngine::Rendering::renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

	drawQuad(transform, color);

}
void GRAVEngine::Rendering::renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, texture2D*& texture, float tilingFactor, const glm::vec4& tintColor)
{
	drawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
}
void GRAVEngine::Rendering::renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, texture2D*& texture, float tilingFactor, const glm::vec4& tintColor)
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

	drawQuad(transform, texture, tilingFactor, tintColor);
}
void GRAVEngine::Rendering::renderer2D::drawQuad(const glm::mat4& transform, const glm::vec4& color)
{
	constexpr size_t quadVertexCount = 4;
	const float textureIndex = 0.0f; // White texture
	constexpr glm::vec2 textureCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };
	const float tilingFActor = 1.0f;

	// See if another batch needs to be made because the max quad count has been reached
	if (s_Data.m_QuadIndexCount >= renderer2DData::s_MaxIndices)
		nextBatch();

	// Get the quad information
	for (size_t i = 0; i < quadVertexCount; i++)
	{
		s_Data.m_QuadVertexBufferPtr->Position = transform * s_Data.m_QuadVertexPositions[i];
		s_Data.m_QuadVertexBufferPtr->Color = color;
		s_Data.m_QuadVertexBufferPtr->TexCoord = textureCoords[i];
		s_Data.m_QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.m_QuadVertexBufferPtr->TilingFactor = tilingFActor;
		s_Data.m_QuadVertexBufferPtr++;
	}

	// Increase the vertice count
	s_Data.m_QuadIndexCount += 6;
	s_Data.m_Stats.m_QuadCount++;
}
void GRAVEngine::Rendering::renderer2D::drawQuad(const glm::mat4& transform, texture2D*& texture, float tilingFactor, const glm::vec4& tintColor)
{
	constexpr size_t quadVertexCount = 4;
	constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

	// See if another batch needs to be made because the amx quad count has been reached
	if (s_Data.m_QuadIndexCount >= renderer2DData::s_MaxIndices)
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
		if (s_Data.m_TextureSlotIndex >= renderer2DData::s_MaxTextureSlots)
			nextBatch();

		textureIndex = (float)s_Data.m_TextureSlotIndex;
		s_Data.m_TextureSlots[s_Data.m_TextureSlotIndex] = texture;
		s_Data.m_TextureSlotIndex++;
	}

	// Get the quad information
	for (size_t i = 0; i < quadVertexCount; i++)
	{
		s_Data.m_QuadVertexBufferPtr->Position = transform * s_Data.m_QuadVertexPositions[i];
		s_Data.m_QuadVertexBufferPtr->Color = tintColor;
		s_Data.m_QuadVertexBufferPtr->TexCoord = textureCoords[i];
		s_Data.m_QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.m_QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.m_QuadVertexBufferPtr++;
	}

	// Increase the vertice count
	s_Data.m_QuadIndexCount += 6;

	s_Data.m_Stats.m_QuadCount++;
}

void GRAVEngine::Rendering::renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
{
	drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
}
void GRAVEngine::Rendering::renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
{
	// Get the rotated quad transform
	glm::mat4 transform = 
		glm::translate(glm::mat4(1.0f), position) * 
		glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) * 
		glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

	drawQuad(transform, color);
}
void GRAVEngine::Rendering::renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, texture2D*& texture, float tilingFactor, const glm::vec4& tintColor)
{
	drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
}
void GRAVEngine::Rendering::renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, texture2D*& texture, float tilingFactor, const glm::vec4& tintColor)
{
	glm::mat4 transform =
		glm::translate(glm::mat4(1.0f), position) *
		glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
		glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

	drawQuad(transform, texture, tilingFactor, tintColor);
}


void GRAVEngine::Rendering::renderer2D::resetStats()
{
	memset(&s_Data.m_Stats, 0, sizeof(statistics));
}
GRAVEngine::Rendering::renderer2D::statistics GRAVEngine::Rendering::renderer2D::getStats()
{
	return s_Data.m_Stats;
}

void GRAVEngine::Rendering::renderer2D::startBatch()
{
	// Reset the quad count and quad pointer
	s_Data.m_QuadIndexCount = 0;
	s_Data.m_QuadVertexBufferPtr = s_Data.m_QuadVertexBufferBase;

	// Start at the default texture again
	s_Data.m_TextureSlotIndex = 1;
}
void GRAVEngine::Rendering::renderer2D::nextBatch()
{
	// Flush the current batch and start a new one
	flush();
	startBatch();
}
