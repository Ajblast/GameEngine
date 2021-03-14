#include "gravpch.h"
#include "platform/opengl/buffers/openglVertexBuffer.h"
#include <glad/glad.h>

GRAVEngine::Rendering::openglVertexBuffer::openglVertexBuffer(uint32 size)
{
    glCreateBuffers(1, &m_RendererID);                              // Create the buffer on the GPU
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);                    // Bind it
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);  // Set its data
}
GRAVEngine::Rendering::openglVertexBuffer::openglVertexBuffer(float* vertices, uint32 size)
{
    glCreateBuffers(1, &m_RendererID);                              // Create the buffer on the GPU
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);                    // Bind it
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);  // Set its data
}
GRAVEngine::Rendering::openglVertexBuffer::~openglVertexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);  // Delete the buffer from the GPU
}

void GRAVEngine::Rendering::openglVertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);    // Bind this buffer
}
void GRAVEngine::Rendering::openglVertexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);               // Bind null buffer
}

void GRAVEngine::Rendering::openglVertexBuffer::setData(const void* data, uint32 size)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);    // Bind this buffer
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);    // Set the data
}