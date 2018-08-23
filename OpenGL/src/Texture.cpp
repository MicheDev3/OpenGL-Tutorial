#include "Texture.h"

#include "Common.h"

#include "stb_image/stb_image.h"


Texture::Texture(const std::string& filepath)
	: m_RendererID(0),m_FilePaht(filepath), m_LocalBuffer(nullptr),
	m_Width(0), m_Height(0), m_BPP(0)
{
	// this call function will flip upside down
	// the texture because OpenGL aspects the 
	// texture to start in the bottom left not 
	// bottom start since PNG store them from up
	// to down
	stbi_set_flip_vertically_on_load(1);
	// 4 for the channels I want -> RGBA
	m_LocalBuffer = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	GLCALL(glGenTextures(1, &m_RendererID));
	GLCALL(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	// Specification how I want to treat the textures
	// I need to specify these parameters, there's not
	// a default (could be but depends on the OpenGL 
	// implementation) and the result would be a black
	// texture
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));                          // GL_TEXTURE_2D -> target texture, GL_TEXTURE_MIN_FILTER -> how OpenGL will resample if it needs to be scale down, GL_LINEAR -> will linear resample anything
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	// should be GL_CLAMP_TO_EDGE but it works for me GL_CLAMP anyway
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCALL(glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height,
		0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer
	));
	GLCALL(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer)
	{
		// I do not need to have a copy of 
		// the texture into the CPU in this
		// case. I made it a local variable
		// in case I want it to store it 
		stbi_image_free(m_LocalBuffer);
	}
}


Texture::~Texture()
{
	// in case the texture has not been free in
	// the constructor I need to free it here
	GLCALL(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot /*= 0*/) const
{
	// since OpenGL is a state machine if I specify
	// a slot every texture binded afterwards are
	// in further slots unless I specify a new slot
	// and they will start again after that new slot
	GLCALL(glActiveTexture(GL_TEXTURE0 + slot));
	GLCALL(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::UnBind() const
{
	GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
}
