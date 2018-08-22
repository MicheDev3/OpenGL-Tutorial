#pragma once

#include "Common.h"

#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"


class Renderer
{
public:
	Renderer();
	~Renderer();

	void Clear() const;
	void Draw(const VertexArray& array, const IndexBuffer& buffer, const Shader& shader) const;
};

