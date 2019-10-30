#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

#include <Assets/CommonAssets.h>
#include <Render/Shader.h>
#include <Render/Texture.h>
#include <vector>
#include <map>

using namespace glm;

class Material
{
public:

	Material()
	{
		this->shader = CommonAssets::instance->defaltErrorShader;
		isShaderInstance = true;
	}

	Material(Shader* shader)
	{
		this->shader = shader;
		isShaderInstance = true;
	}

	Material(string vsPath, string fsPath)
	{
		this->shader_unique = make_unique<Shader>(vsPath, fsPath);
		this->shader = shader_unique.get();
		//this->shader = dynamic_cast<Shader*>(this);
		isShaderInstance = false;
	}
	
	//~Material()
	//{

	//}


	void SetModelMat(glm::mat4 mat)
	{
		mat4Params["model"] = mat;
	}

	// texture
	void AddTexture(Texture &texture)
	{
		int m_textureSlot = -1;
		for (int i = 0; i < maxTextureCount; i++)
		{
			if (textures[i].id == 0)
			{
				m_textureSlot = i;
				textures[m_textureSlot] = texture;
				SetParam(textures[m_textureSlot].keyword, m_textureSlot);
				break;
			}
		}
		if (m_textureSlot < 0)
		{
			cout << "Reached to the max texture count for material " << shader->fsPath.c_str() << endl;
			return;
		}

	}

	void ClearTextrues()
	{
		for (int i = 0; i < maxTextureCount; i++)
		{
				textures[i].Reset();
		}
	}

	// Remove texture from material's texture list, do not call it in main loop
	void RemoveTexture(Texture &texture)
	{
		for (int i = 0; i < maxTextureCount; i++)
		{
			if (textures[i].id == texture.id)
				textures[i].Reset();
		}
	}

	void BindTextures()
	{
		for (int i = 0; i < maxTextureCount; i++)
		{
			if (textures[i].id != 0)
			{
				glActiveTexture(GL_TEXTURE0 + i);
				textures[i].Bind();
			}
			else
			{
				glActiveTexture(GL_TEXTURE0);
				break;
			}
		}
	}

	void ReplaceTexture(Texture& pre_texture, Texture& texture)
	{
		unsigned int tar_ID = pre_texture.id;
		unsigned int ID = texture.id;
		if (tar_ID != ID)
		{
			for (int i = 0; i < maxTextureCount; i++)
			{
				if (textures[i].id = tar_ID)
				{
					textures[i] = texture;
					return;
				}
			}
		}
	}
	void use()
	{
		this->shader->use();
		UpdateParams();
		BindTextures();
	}

	void ChangeShader(Shader* shader)
	{
		this->shader = shader;
	}

	// set uniform values
	void SetParam(const string &name, bool value)
	{
		boolParams[name] = value;
		//boolParams.insert(std::pair<string, bool>(name, value));
	}
	void SetParam(const string &name, int value)
	{
		intParams[name] = value;
	}
	void SetParam(const string &name, float value)
	{
		floatParams[name] = value;
	}
	void SetParam(const string &name, float x, float y, float z)
	{
		vec3Params[name] = vec3(x, y, z);
	}
	void SetParam(const string &name, glm::vec3 vec3)
	{
		vec3Params[name] = vec3;
	}
	void SetParam(const string &name, float x, float y, float z, float w)
	{
		vec4Params[name] = vec4(x, y, z, w);
	}
	void SetParam(const string &name, glm::vec4 vec4)
	{
		vec4Params[name] = vec4;
	}
	void SetParam(const string &name, glm::mat4 mat)
	{
		mat4Params[name] = mat;
	}

private:

	void UpdateParams()
	{
		for (map<string, bool>::iterator it = boolParams.begin(); it != boolParams.end(); it++)
		{
			shader->setBool(it->first, it->second);
		}
		for (map<string, float>::iterator it = floatParams.begin(); it != floatParams.end(); it++)
		{
			shader->setFloat(it->first, it->second);
		}
		for (map<string, int>::iterator it = intParams.begin(); it != intParams.end(); it++)
		{
			shader->setInt(it->first, it->second);
		}
		for (map<string, vec2>::iterator it = vec2Params.begin(); it != vec2Params.end(); it++)
		{
			shader->setVec2f(it->first, it->second);
		}
		for (map<string, vec3>::iterator it = vec3Params.begin(); it != vec3Params.end(); it++)
		{
			shader->setVec3f(it->first, it->second);
		}
		for (map<string, vec4>::iterator it = vec4Params.begin(); it != vec4Params.end(); it++)
		{
			shader->setVec4f(it->first, it->second);
		}
		for (map<string, mat4>::iterator it = mat4Params.begin(); it != mat4Params.end(); it++)
		{
			shader->setMat4f(it->first, it->second);
		}
	}

public:

	static const int maxTextureCount = 16;

private:

	bool isShaderInstance = true;

	map<string, bool> boolParams;
	map<string, float> floatParams;
	map<string, int> intParams;
	map<string, vec2> vec2Params;
	map<string, vec3> vec3Params;
	map<string, vec4> vec4Params;
	map<string, mat4> mat4Params;

	std::unique_ptr<Shader> shader_unique;
	Shader* shader;
	Texture textures[maxTextureCount];
};

#endif // !MATERIAL_H



