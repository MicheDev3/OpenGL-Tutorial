#pragma once

#include <string>

#include "Common.h"

class Texture
{
public:
	Texture(const std::string& filepath);
	~Texture();

	void Bind(unsigned int slot = 0) const;                                                    // slot allow me to specify which slot to attach the texture (slots depends on the GPU and their implementations)
	void UnBind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }

private:
	unsigned int m_RendererID;
	std::string m_FilePaht;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;                                                             // m_BPP bit per pixels

};

