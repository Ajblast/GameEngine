#include "gravpch.h"
#include "renderer2D.h"

#include "rendering/vertexArray.h"
#include "rendering/shaders/shader.h"
#include "rendering/shaders/shaderDataType.h"
#include "rendering/renderer/rendererCommand.h"

#include <glm/gtc/matrix_transform.hpp>

GRAVEngine::Locks::spinLock GRAVEngine::Rendering::renderer2D::s_BatchLock = GRAVEngine::Locks::spinLock();

// A vertex of a quad
struct quadVertex
{
	glm::vec3 Position;
	glm::vec4 Color;
	glm::vec2 TexCoord;
	float TexIndex;
	float TilingFactor;
};

struct quadBatch
{
	// Constants
	static const GRAVEngine::uint32 s_MaxQuads = 20000;
	static const GRAVEngine::uint32 s_MaxVertices = s_MaxQuads * 4;
	static const GRAVEngine::uint32 s_MaxIndices = s_MaxQuads * 6;
	static const GRAVEngine::uint32 s_MaxTextureSlots = 32; // TODO: RenderCaps

	quadBatch() = default;
	quadBatch(quadBatch&& other) noexcept :
		m_QuadVertexBuffer(std::move(other.m_QuadVertexBuffer)), m_IndexBuffer(std::move(other.m_IndexBuffer)), m_QuadIndexCount(std::move(other.m_QuadIndexCount)),
		m_QuadVertexBufferBase(std::move(other.m_QuadVertexBufferBase)), m_QuadVertexBufferPtr(std::move(other.m_QuadVertexBufferPtr)),
		m_TextureSlots(std::move(other.m_TextureSlots)), m_TextureSlotIndex(std::move(other.m_TextureSlotIndex))
	{}

	GRAVEngine::ref<GRAVEngine::Rendering::vertexBuffer> m_QuadVertexBuffer;							// The quad vertex buffer
	GRAVEngine::ref<GRAVEngine::Rendering::indexBuffer> m_IndexBuffer;									// The quad index buffer

	GRAVEngine::uint32 m_QuadIndexCount = 0;															// The current quad count
	GRAVEngine::scope<quadVertex[]> m_QuadVertexBufferBase = nullptr;									// Array of quad vertexes
	quadVertex* m_QuadVertexBufferPtr = nullptr;														// The current quad vertex

	std::array<GRAVEngine::ref<GRAVEngine::Rendering::texture2D>, s_MaxTextureSlots> m_TextureSlots;	// Array of textures
	GRAVEngine::uint32 m_TextureSlotIndex = 1;															// 0 = white texture
};

// Data used for the renderer2D
struct renderer2DData
{
	std::vector<quadBatch> m_Batches;											// All of the batches

	GRAVEngine::ref<GRAVEngine::Rendering::shader> m_TextureShader;				// The rendering texture shader

	GRAVEngine::ref<GRAVEngine::Rendering::texture2D>		m_WhiteTexture;		// The white default texture

	glm::vec4 m_QuadVertexPositions[4] = {
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
	};	// The default quad vertex positions

	GRAVEngine::Rendering::renderer2D::statistics m_Stats;	// Stats for drawing quads
};

// TODO: Determine if this truly needs to be static and free floating here. Should probably be removed
static renderer2DData s_Data;

void GRAVEngine::Rendering::renderer2D::startup()
{
	GRAV_PROFILE_FUNCTION();

	// Create the white texture and set its data
	s_Data.m_WhiteTexture = rendererAPI::getInstance().createTexture(1, 1);
	uint32 whiteTextureData = 0xffffffff;
	s_Data.m_WhiteTexture->setData(&whiteTextureData, sizeof(uint32));

	// Set the sampler indices
	uint32 batchMaxTextures = quadBatch::s_MaxTextureSlots;
	int32 samplers[quadBatch::s_MaxTextureSlots];
	for (uint32 i = 0; i < batchMaxTextures; i++)
		samplers[i] = i;

	// Create the textured shader
	s_Data.m_TextureShader = rendererAPI::getInstance().createShader("assets/shaders/texture.glsl");
	s_Data.m_TextureShader->bind();
	s_Data.m_TextureShader->setIntArray("u_Textures", samplers, batchMaxTextures);

	// Set the default quad positions
	s_Data.m_QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
	s_Data.m_QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
	s_Data.m_QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
	s_Data.m_QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
}
void GRAVEngine::Rendering::renderer2D::shutdown()
{
	GRAV_PROFILE_FUNCTION();

	// Lock the batches because we alter it
	Locks::scopedLock<decltype(s_BatchLock)> lock(s_BatchLock);

	// Clear all of the batches
	s_Data.m_Batches.clear();
}

void GRAVEngine::Rendering::renderer2D::beginScene(const camera& camera)
{
	GRAV_PROFILE_FUNCTION();

	// Get the projection of the camera
	//glm::mat4 viewProj = camera.getProjectionMatrix() * glm::inverse(transform);

	startBatch();

	// Set the view matrix
	s_Data.m_TextureShader->bind();
	s_Data.m_TextureShader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());
}
void GRAVEngine::Rendering::renderer2D::endScene()
{
	GRAV_PROFILE_FUNCTION();

	// Don't actually do anything here because the application will handle the flushing
}
void GRAVEngine::Rendering::renderer2D::flush()
{
	GRAV_PROFILE_FUNCTION();

	// Lock the batches because we alter it
	Locks::scopedLock<decltype(s_BatchLock)> lock(s_BatchLock);

	// Draw the quads for every batch
	for (auto it = s_Data.m_Batches.begin(); it != s_Data.m_Batches.end(); it++)
	{
		quadBatch& batch = *it;

		if (batch.m_QuadIndexCount == 0)
			continue; // Nothing to draw

		// Get the size of the data
		uint32 dataSize = (uint32)((uint8*)batch.m_QuadVertexBufferPtr - (uint8*)batch.m_QuadVertexBufferBase.get());
		batch.m_QuadVertexBuffer->setData(batch.m_QuadVertexBufferBase.get(), dataSize);

		// Bind the textures
		for (uint32 i = 0; i < batch.m_TextureSlotIndex; i++)
			batch.m_TextureSlots[i]->bind(i);


		// Create the data VAO
		ref<vertexArray> quadVertexArray = rendererAPI::getInstance().createVertexArray();
		
		// Add the vertex buffer and the index buffer
		quadVertexArray->addVertexBuffer(batch.m_QuadVertexBuffer);
		quadVertexArray->setIndexBuffer(batch.m_IndexBuffer);

		quadVertexArray->bind();
		s_Data.m_TextureShader->bind();

		// Draw the indexed items
		rendererCommand::drawIndexed(quadVertexArray, batch.m_QuadIndexCount);

		s_Data.m_Stats.m_DrawCalls++;
	}

	// Clear all of the batches
	s_Data.m_Batches.clear();
}

void GRAVEngine::Rendering::renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
	drawQuad({ position.x, position.y, 0.0f }, size, color);
}
void GRAVEngine::Rendering::renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
	GRAV_PROFILE_FUNCTION();

	glm::mat4 transform = glm::translate(glm::mat4(1.0), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

	drawQuad(transform, color);

}
void GRAVEngine::Rendering::renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const ref<texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
{
	GRAV_PROFILE_FUNCTION();

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

	drawQuad(transform, texture, tilingFactor, tintColor);
}
void GRAVEngine::Rendering::renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const ref<texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
{
	drawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
}
void GRAVEngine::Rendering::renderer2D::drawQuad(const glm::mat4& transform, const glm::vec4& color)
{
	GRAV_PROFILE_FUNCTION();

	// Lock the batches because we alter it
	Locks::scopedLock<decltype(s_BatchLock)> lock(s_BatchLock);

	constexpr size_t quadVertexCount = 4;
	const float textureIndex = 0.0f; // White texture
	constexpr glm::vec2 textureCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };
	const float tilingFActor = 1.0f;

	// See if another batch needs to be made because the max quad count has been reached
	if (s_Data.m_Batches.back().m_QuadIndexCount >= quadBatch::s_MaxIndices)
		nextBatch();

	quadBatch& batch = s_Data.m_Batches.back();

	// Get the quad information
	for (size_t i = 0; i < quadVertexCount; i++)
	{
		batch.m_QuadVertexBufferPtr->Position = transform * s_Data.m_QuadVertexPositions[i];
		batch.m_QuadVertexBufferPtr->Color = color;
		batch.m_QuadVertexBufferPtr->TexCoord = textureCoords[i];
		batch.m_QuadVertexBufferPtr->TexIndex = textureIndex;
		batch.m_QuadVertexBufferPtr->TilingFactor = tilingFActor;
		batch.m_QuadVertexBufferPtr++;
	}

	// Increase the vertice count
	batch.m_QuadIndexCount += 6;
	s_Data.m_Stats.m_QuadCount++;
}
void GRAVEngine::Rendering::renderer2D::drawQuad(const glm::mat4& transform, const ref<texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
{
	GRAV_PROFILE_FUNCTION();

	// Lock the batches because we alter it
	Locks::scopedLock<decltype(s_BatchLock)> lock(s_BatchLock);

	constexpr size_t quadVertexCount = 4;
	constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

	// See if another batch needs to be made because the amx quad count has been reached
	if (s_Data.m_Batches.back().m_QuadIndexCount >= quadBatch::s_MaxIndices)
		nextBatch();

	quadBatch& batch = s_Data.m_Batches.back();

	// Set the texture index values
	float textureIndex = 0.0f;
	for (uint32_t i = 1; i < batch.m_TextureSlotIndex; i++)
	{
		if (*batch.m_TextureSlots[i] == *texture)
		{
			textureIndex = (float)i;
			break;
		}
	}

	// If the texture index is null
	if (textureIndex == 0.0f)
	{
		// Start a new batch because there are no more texture slots available
		if (batch.m_TextureSlotIndex >= quadBatch::s_MaxTextureSlots)
			nextBatch();

		textureIndex = (float)batch.m_TextureSlotIndex;
		batch.m_TextureSlots[batch.m_TextureSlotIndex] = texture;
		batch.m_TextureSlotIndex++;
	}

	// Get the quad information
	for (size_t i = 0; i < quadVertexCount; i++)
	{
		batch.m_QuadVertexBufferPtr->Position = transform * s_Data.m_QuadVertexPositions[i];
		batch.m_QuadVertexBufferPtr->Color = tintColor;
		batch.m_QuadVertexBufferPtr->TexCoord = textureCoords[i];
		batch.m_QuadVertexBufferPtr->TexIndex = textureIndex;
		batch.m_QuadVertexBufferPtr->TilingFactor = tilingFactor;
		batch.m_QuadVertexBufferPtr++;
	}

	// Increase the vertice count
	batch.m_QuadIndexCount += 6;

	s_Data.m_Stats.m_QuadCount++;
}

void GRAVEngine::Rendering::renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
{
	drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
}
void GRAVEngine::Rendering::renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
{
	GRAV_PROFILE_FUNCTION();

	// Get the rotated quad transform
	glm::mat4 transform = 
		glm::translate(glm::mat4(1.0f), position) * 
		glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) * 
		glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

	drawQuad(transform, color);
}
void GRAVEngine::Rendering::renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const ref<texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
{
	drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
}
void GRAVEngine::Rendering::renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const ref<texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
{
	GRAV_PROFILE_FUNCTION();

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
	quadBatch batch;

	// Create the vertex buffer for the vertex data
	batch.m_QuadVertexBuffer = rendererAPI::getInstance().createVertexBuffer(batch.s_MaxVertices * sizeof(quadVertex));
	batch.m_QuadVertexBuffer->setLayout({
		{shaderDataType::FLOAT3, "a_Position"},
		{shaderDataType::FLOAT4, "a_Color"},
		{shaderDataType::FLOAT2, "a_TexCoord"},
		{shaderDataType::FLOAT, "a_TexIndex"},
		{shaderDataType::FLOAT, "a_TilingFactor"}
		});

	// Get the base pointer for the quad vertices
	batch.m_QuadVertexBufferBase = createScope<quadVertex[]>(batch.s_MaxVertices);

	// Get the base index for the quad indices
	scope<uint32[]> quadIndices = createScope<uint32[]>(batch.s_MaxIndices);

	// Create all the possible indices for all of the triangles in the quads
	uint32 offset = 0;
	for (uint32 i = 0; i < batch.s_MaxIndices; i += 6)
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
	batch.m_IndexBuffer = rendererAPI::getInstance().createIndexBuffer(quadIndices.get(), batch.s_MaxIndices);

	// Set first texture slot to 0
	batch.m_TextureSlots[0] = s_Data.m_WhiteTexture;


	// Reset the quad count and quad pointer
	batch.m_QuadIndexCount = 0;
	batch.m_QuadVertexBufferPtr = batch.m_QuadVertexBufferBase.get();

	// Start at the default texture again
	batch.m_TextureSlotIndex = 1;

	{
		// Lock the batches because we alter it
		Locks::scopedLock<decltype(s_BatchLock)> lock(s_BatchLock);

		s_Data.m_Batches.push_back(std::move(batch));
	}
}
void GRAVEngine::Rendering::renderer2D::nextBatch()
{
	// Start a new batch. This will create a new batch and add it to the list of batches
	startBatch();
}
