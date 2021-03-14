#include "gravpch.h"
#include "platform/opengl/buffers/openglIndexBuffer.h"
#include <glad/glad.h>

GRAVEngine::Rendering::openglIndexBuffer::openglIndexBuffer(uint32* indices, uint32 count) : m_Count(count)
{
    glCreateBuffers(1, &m_RendererID);                              // Create the buffer on the GPU
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);                    // Bind it
    glBufferData(GL_ARRAY_BUFFER, count*sizeof(uint32), indices, GL_STATIC_DRAW);  // Set its data
}

GRAVEngine::Rendering::openglIndexBuffer::~openglIndexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);  // Delete the buffer from the GPU
}

void GRAVEngine::Rendering::openglIndexBuffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);    // Bind this buffer
}

void GRAVEngine::Rendering::openglIndexBuffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);               // Bind null buffer
}