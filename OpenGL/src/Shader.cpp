#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Common.h"


Shader::Shader(const std::string& filepath)
	:m_FilePath(filepath), m_RendererID(0)
{
	ShaderProgramSources source = ParseShader(filepath);
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);

}


Shader::~Shader()
{
	GLCALL(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
	GLCALL(glUseProgram(m_RendererID));
}

void Shader::UnBind() const
{
	GLCALL(glUseProgram(0));
}

void Shader::setUniform1i(const std::string& name, int value)
{
	GLCALL(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{

	GLCALL(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
	{
		m_UniformLocationCache[name];
	}
		

	GLCALL(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
	{
		std::cout << "Warning: uniform '" << name << "' doesn't exist!\n";
	}

	m_UniformLocationCache[name] = location;
	return location;
}


ShaderProgramSources Shader::ParseShader(const std::string& filePath)
{
	std::ifstream stream(filePath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			} else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		} else
		{
			int index = static_cast<int>(type);
			ss[index] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	GLCALL(unsigned int id = glCreateShader(type));
	const char* src = source.c_str();
	GLCALL(glShaderSource(id, 1, &src, nullptr));
	GLCALL(glCompileShader(id));

	int result;
	GLCALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLCALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		// I want to allocate the message in the stack not in the heap
		// but cannot do this char message[length] so I can do this
		char* message = (char*)alloca(length * sizeof(char));
		GLCALL(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Failed to compile " <<
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<
			" shader\n";
		std::cout << message << '\n';

		// since shader did not work delete it
		GLCALL(glDeleteShader(id));
		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLCALL(unsigned int program = glCreateProgram());
	// compiling the shader for attaching the the program context
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	// attaching and linking the shaders
	GLCALL(glAttachShader(program, vs));
	GLCALL(glAttachShader(program, fs));
	GLCALL(glLinkProgram(program));
	// validating the shaders in order to check
	// if the shaders attached are corrects
	GLCALL(glValidateProgram(program));

	// after attach and link the shaders to the program context
	// I can remove the shaders intermediates like the intermediate
	// when compiling c++ source code (.OBJ)
	GLCALL(glDeleteShader(vs));
	GLCALL(glDeleteShader(fs));

	return program;
}
