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
#include <unordered_map>
#include <Data/Enums.h>
#include <Debug/Debug.h>

using namespace std;

class Shader
{

public:
	// id for shader program
	unsigned int ID;

	static std::unordered_map<int, Shader*> loadedShaders;
	static std::unordered_map<int, Shader*> activeShaders;

	string vsPath;
	string fsPath;

	// constraction
private:

	static Shader* errorShader;
	unordered_map<string, int> cachedUniformLocation;
	bool noError = false;

public:

	Shader(const string& vsPath, const string& fsPath, bool isProp = false);

	~Shader();

	// use shader program
	virtual void use();

	// reload shader
	bool Reload();

	// set uniform values
	void setBool(const std::string &name, bool value);
	void setInt(const std::string &name, int value);
	void setFloat(const std::string &name, float value);
	void setVec2f(const std::string &name, glm::vec2& vec2);
	void setVec3f(const std::string &name, float x, float y, float z);
	void setVec3f(const std::string &name, glm::vec3& vec3);
	void setVec4f(const std::string &name, float x, float y, float z, float w);
	void setVec4f(const std::string &name, glm::vec4& vec4);
	void setMat4f(const std::string &name, glm::mat4& mat);


	void SetParam(const std::string &name, bool value);
	void SetParam(const std::string &name, int value);
	void SetParam(const std::string& name, unsigned int value);
	void SetParam(const std::string &name, float value);
	void SetParam(const std::string& name, glm::vec2& vec2);
	void SetParam(const std::string &name, float x, float y, float z);
	void SetParam(const std::string &name, glm::vec3& vec3);
	void SetParam(const std::string &name, float x, float y, float z, float w);
	void SetParam(const std::string &name, glm::vec4& vec4);
	void SetParam(const std::string &name, glm::mat4& mat);

	void BindTexture(const char* paramname, unsigned int id, unsigned int type = GL_TEXTURE_2D);

	bool isProp = false;

protected:

	bool CreateShaderProgram();

	void ChangeErrorShader(Shader* errShader);

private:
	
	std::unordered_map<const char*,unsigned int> slot ;
	unsigned int occupiedSlot = 0;
	int GetCachedUniformLocation(const string& name);

};

#endif
