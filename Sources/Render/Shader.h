// 2019-01-28 14:13:24 chao

#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <File/filesystem.h>
#include <map>

using namespace std;

class Shader
{

public:
	// id for shader program
	unsigned int ID;

	static map<int, Shader*> loadedShaders;

	// constraction

	Shader(const string& vsPath, const string& fsPath, bool isSceneShader = false)
	{
		this->vsPath = vsPath;
		this->fsPath = fsPath;
		CreateShaderProgram();
		isUseInScene = isSceneShader;
		Shader::loadedShaders[ID] = this;
	}

	~Shader()
	{
		if(!Shader::loadedShaders.empty() && this != NULL)
			Shader::loadedShaders.erase(this->ID);
	}

	// use shader program
	virtual void use()
	{
		glUseProgram(ID);
	}
	// set uniform values
	void setBool(const std::string &name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string &name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setVec2f(const std::string &name, glm::vec2 vec2) const
	{
		glUniform2f(glGetUniformLocation(ID, name.c_str()), vec2.x, vec2.y);
	}
	void setVec3f(const std::string &name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}
	void setVec3f(const std::string &name, glm::vec3 vec3) const
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), vec3.x, vec3.y, vec3.z);
	}
	void setVec4f(const std::string &name, float x, float y, float z, float w) const
	{
		glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
	}
    void setVec4f(const std::string &name, glm::vec4 vec4) const
    {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), vec4.x, vec4.y, vec4.z, vec4.w);
    }
	void setMat4f(const std::string &name, glm::mat4 mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID,name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	}


	void SetParam(const std::string &name, bool value) const
	{
		setBool(name, value);
	}
	void SetParam(const std::string &name, int value) const
	{
		setInt(name, value);
	}
	void SetParam(const std::string &name, float value) const
	{
		setFloat(name, value);
	}
	void SetParam(const std::string &name, float x, float y, float z) const
	{
		setVec3f(name, x, y, z);
	}
	void SetParam(const std::string &name, glm::vec3 vec3) const
	{
		setVec3f(name, vec3);
	}
	void SetParam(const std::string &name, float x, float y, float z, float w) const
	{
		setVec4f(name, x, y, z, w);
	}
	void SetParam(const std::string &name, glm::vec4 vec4) const
	{
		setVec4f(name, vec4);
	}
	void SetParam(const std::string &name, glm::mat4 mat) const
	{
		setMat4f(name, mat);;
	}

	bool isUseInScene = false;

protected:

	void CreateShaderProgram()
	{
		string vertexPath = FileSystem::getPath(vsPath);
		string fragmentPath = FileSystem::getPath(fsPath);

		// 1. get shader source file from a gaven path
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// make sure throw a exception while open failed：
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open file from path
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// pass buffer to file stream
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file after read
			vShaderFile.close();
			fShaderFile.close();
			// convert stream to string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
			std::cout << vertexPath << fragmentPath << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		// 22. compile shader
		unsigned int vertex, fragment;
		int success;
		char infoLog[512];

		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		// check if compile success
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			std::cout << vertexPath << std::endl;
		};

		// fragment shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		// check if compile success
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
			std::cout << fragmentPath << std::endl;
		}

		// shader program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		// check if link success
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		// release resource
		glDeleteShader(vertex);
		glDeleteShader(fragment);

	}

	void ChangeErrorShader(Shader* errShader)
	{
		Shader::errorShader = errShader;
	}

private:

	static Shader* errorShader;

public:

	string vsPath;
	string fsPath;

};

#endif
