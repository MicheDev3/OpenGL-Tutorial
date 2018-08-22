#include "Common.h"
#include "IndexBuffer.h"


IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	:m_Count(count)
{
	// making sure if they have the same size, they might
	// but not sure, this to avoid to use GLuint
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));


	GLCALL(glGenBuffers(1, &m_RendererID));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() 
{
	GLCALL(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::UnBind() const
{
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
