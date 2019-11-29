#pragma once
#ifndef Debug_H
#define Debug_H

#ifdef CHAOGL_DEBUG
	#define glCheckError() glCheckError_(__FILE__, __LINE__)
#else  
	#define glCheckError()
#endif

#ifdef WIN32

#define ASSERT(x) if (!(x)) __debugbreak()

#endif // WIN32

#define GLCall(x) glClearError();\
					x;\
					ASSERT(glLogError(#x, __FILE__, __LINE__))

#include <glad/glad.h>
#include <string>
#include <iostream>

static void glClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool glLogError(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OPENGL_ERROR] " << error <<" in " << function<< " file "<< file<< " line["<< line << "]. "<<std::endl;
		return false;
	}
	return true;
}

class Debug
{
public:
	Debug();
	~Debug();

	GLenum glCheckError_(const char *file, int line)
	{
		GLenum errorCode;
		while ((errorCode = glGetError()) != GL_NO_ERROR)
		{
			std::string error;
			switch (errorCode)
			{
			case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
			case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
			case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
			case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
			case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
			case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
			}
			std::cout << error << " | " << file << " (" << line << ")" << std::endl;
		}
		return errorCode;
	}
};

#endif // !DEBUG_H
