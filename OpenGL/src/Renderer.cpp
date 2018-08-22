#include "Renderer.h"



Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}

void Renderer::Clear() const
{
	GLCALL(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	va.Bind();
	ib.Bind();
	shader.Bind();
	GLCALL(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}
