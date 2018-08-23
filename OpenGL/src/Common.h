#pragma once
#include <iostream>

#include <GL/glew.h>     // since glfw include glew must be put first


// MSVC compiler dependent
#define ASSERT(x) if (!(x)) __debugbreak();  
#define GLCALL(x) GLCleanError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__));\

static void GLCleanError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL error] (" << error << ") " << function << " " << file << ": " << line << '\n';
		return false;
	}
	return true;
}

#if 0
// this implementation would not work for wrapping 
// around variable declaration I must use the one up
#define GLCALL(x) GLCleanError();\
	do\
	{\
		x;\
		ASSERT(GLLogCall(#x, __FILE__, __LINE__));\
	}\
	while(false)
#endif