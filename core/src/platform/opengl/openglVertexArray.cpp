#include "gravpch.h"
#include "platform/opengl/openglVertexArray.h"
#include <glad/glad.h>

namespace GRAVEngine
{
    namespace Rendering
    {
        static GLenum shaderDataTypeToOpenGLBaseType(shaderDataType type)
        {
			switch (type)
			{
			case shaderDataType::FLOAT:    return GL_FLOAT;
			case shaderDataType::FLOAT2:   return GL_FLOAT;
			case shaderDataType::FLOAT3:   return GL_FLOAT;
			case shaderDataType::FLOAT4:   return GL_FLOAT;
			case shaderDataType::MAT3:     return GL_FLOAT;
			case shaderDataType::MAT4:     return GL_FLOAT;
			case shaderDataType::INT:      return GL_INT;
			case shaderDataType::INT2:     return GL_INT;
			case shaderDataType::INT3:     return GL_INT;
			case shaderDataType::INT4:     return GL_INT;
			case shaderDataType::BOOL:     return GL_BOOL;
			}

			// Unknown shader data type
			GRAV_ASSERT(false);
			return 0;
        }
	}
}

GRAVEngine::Rendering::openglVertexArray::openglVertexArray()
{
	glCreateVertexArrays(1, &m_RendererID);	// Create a vertex array on the GPU
}

GRAVEngine::Rendering::openglVertexArray::~openglVertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID);	// Delete the vertex array on the GPU
}

void GRAVEngine::Rendering::openglVertexArray::bind() const
{
	glBindVertexArray(m_RendererID);		// Bind this vertex array to the GPU
}

void GRAVEngine::Rendering::openglVertexArray::unbind() const
{
	glBindVertexArray(0);					// Unbind this vertex array from the GPU
}

void GRAVEngine::Rendering::openglVertexArray::addVertexBuffer(const ref<vertexBuffer>& vertexBuffer)
{
	// Assure that the vertex buffer has a layout
	GRAV_ASSERT(vertexBuffer->getLayout().getElements().size());

	bind();	// Bind this vertex Buffer
	vertexBuffer->bind();	// Bind the added vertex buffer

	const auto layout = vertexBuffer->getLayout();
	for (auto& element : layout)
	{
		switch (element.m_Type)
		{
			// All these cases use the same code
			case shaderDataType::FLOAT:
			case shaderDataType::FLOAT2:
			case shaderDataType::FLOAT3:
			case shaderDataType::FLOAT4:
			{
				// Enable the vertex buffer's index buffer
				glEnableVertexAttribArray(m_VertexBufferIndex);

				// Add a pointer to the vertex buffer
				glVertexAttribPointer(m_VertexBufferIndex,
					element.getComponentCount(),
					shaderDataTypeToOpenGLBaseType(element.m_Type),
					element.m_Normalized ? GL_TRUE : GL_FALSE,
					layout.getStride(),
					(const void*)element.m_Offset);

				// Increate the vertex buffer's index
				m_VertexBufferIndex++;
				break;
			}
			case shaderDataType::INT:
			case shaderDataType::INT2:
			case shaderDataType::INT3:
			case shaderDataType::INT4:
			case shaderDataType::BOOL:
			{
				// Enable the vertex buffer's index buffer
				glEnableVertexAttribArray(m_VertexBufferIndex);

				// Add a pointer to the vertex buffer
				glVertexAttribIPointer(m_VertexBufferIndex,
					element.getComponentCount(),
					shaderDataTypeToOpenGLBaseType(element.m_Type),
					layout.getStride(),
					(const void*)element.m_Offset);

				// Increate the vertex buffer's index
				m_VertexBufferIndex++;
				break;
			}
			case shaderDataType::MAT3:
			case shaderDataType::MAT4:
			{
				uint8_t count = element.getComponentCount();
				for (uint8_t i = 0; i < count; i++)
				{
					// Enable the vertex buffer's index buffer
					glEnableVertexAttribArray(m_VertexBufferIndex);

					// Add a pointer to the vertex buffer
					glVertexAttribPointer(m_VertexBufferIndex,
						count,
						shaderDataTypeToOpenGLBaseType(element.m_Type),
						element.m_Normalized ? GL_TRUE : GL_FALSE,
						layout.getStride(),
						(const void*)(element.m_Offset + sizeof(float) * count * i));
					glVertexAttribDivisor(m_VertexBufferIndex, 1);
					m_VertexBufferIndex++;
				}
				break;
			}
			default:
				// Unknown shader data type
				GRAV_ASSERT(false);
		}
	}

	m_VertexBuffers.push_back(vertexBuffer);
}

void GRAVEngine::Rendering::openglVertexArray::setIndexBuffer(const ref<indexBuffer>& indexBuffer)
{
	bind();	// Bind this vertex array
	indexBuffer->bind();

	m_IndexBuffer = indexBuffer;
}
