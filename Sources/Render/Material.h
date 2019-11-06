#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

#include <Assets/CommonAssets.h>
#include <Render/Shader.h>
#include <Render/Texture.h>
#include <vector>
#include <map>
#include <filesystem>

class Material
{
public:

	Material();

	Material(Shader* shader);

	Material(Shader* shader, string texturePath);

	Material(string vsPath, string fsPath);
	
	//~Material()
	//{

	//}


	void SetModelMat(glm::mat4 mat);

	// texture
	void AddTexture(Texture &texture);

	void ClearTextrues();

	// Remove texture from material's texture list, do not call it in main loop
	void RemoveTexture(Texture &texture);

	void BindTextures();

	void ReplaceTexture(Texture& texture);
	void ReplaceTexture(Texture& pre_texture, Texture& texture);
	void use();

	void ChangeShader(Shader* shader)
	{
		this->shader = shader;
	}

	// set uniform values
	void SetParam(const string &name, bool value);
	void SetParam(const string &name, int value);
	void SetParam(const string &name, float value);
	void SetParam(const string &name, float x, float y, float z);
	void SetParam(const string &name, glm::vec3 vec3);
	void SetParam(const string &name, float x, float y, float z, float w);
	void SetParam(const string &name, glm::vec4 vec4);
	void SetParam(const string &name, glm::mat4 mat);

private:

	void UpdateParams();

	void LoadDefaultTexture(vector<Texture>& textures, Texture::TextureType type);

	Texture LoadTexture(string path, Texture::TextureType type);

public:

	static const int maxTextureCount = 16;

private:

	bool isShaderInstance = true;

	map<string, bool> boolParams;
	map<string, float> floatParams;
	map<string, int> intParams;
	map<string, glm::vec2> vec2Params;
	map<string, glm::vec3> vec3Params;
	map<string, glm::vec4> vec4Params;
	map<string, glm::mat4> mat4Params;

	std::unique_ptr<Shader> shader_unique;
	Shader* shader;
	Texture textures[maxTextureCount];
};

#endif // !MATERIAL_H



