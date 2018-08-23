#pragma once

// forward declaration
class Shader;
class VertexArray;
class IndexBuffer;


class Renderer
{
public:
	Renderer();
	~Renderer();

	void Clear() const;
	void Draw(const VertexArray& array, const IndexBuffer& buffer, const Shader& shader) const;
};

