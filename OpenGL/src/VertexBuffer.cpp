#include "Common.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	GLCALL(glGenBuffers(1, &m_RendererID));                               // creating the buffer with a specific id
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));                  // binding the buffer in order to select it an do stuff
	GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));    // defining the data and specs for drawing
}

VertexBuffer::~VertexBuffer()
{
	GLCALL(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::UnBind() const
{
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
