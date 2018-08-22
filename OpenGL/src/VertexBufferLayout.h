#pragma once

#include <vector>

#include "Common.h"


struct VertexBufferElement 
{
	unsigned int type;
	unsigned int count;
	unsigned int normalize;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT:			return 4;
			case GL_UNSIGNED_INT:   return 4;
			case GL_UNSIGNED_BYTE:  return 1;
		}
		ASSERT(false);
		return 0;
	}

};

class VertexBufferLayout
{
	using Elements = std::vector<VertexBufferElement>;

public:
	VertexBufferLayout()
		:m_Stride(0)
	{}

	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);                                                   // I should make a function base on the OpenGL type in order to get the size of it
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline unsigned int GetStride() const { return m_Stride; }
	inline const Elements& GetElements() const { return m_Elements; }

private:
	Elements m_Elements;
	unsigned int m_Stride;

};

