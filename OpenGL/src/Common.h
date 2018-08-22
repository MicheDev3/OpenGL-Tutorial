#pragma once
#include <iostream>

#include <GL/glew.h>     // since glfw include glew must be put first
#include <GLFW/glfw3.h>  // does it make sense to include it just one time? I think it does


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