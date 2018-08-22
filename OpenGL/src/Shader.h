#pragma once

#include <string>
#include <unordered_map>


struct ShaderProgramSources
{
	std::string VertexSource, FragmentSource;
};

class Shader
{
	using LocationCache = std::unordered_map<std::string, int>;

public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void UnBind() const;

	void setUniform1i(const std::string& name, int value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private:
	int GetUniformLocation(const std::string& name);

	ShaderProgramSources ParseShader(const std::string& filePath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

private:
	std::string m_FilePath;
	unsigned int m_RendererID;

	LocationCache m_UniformLocationCache;

};

