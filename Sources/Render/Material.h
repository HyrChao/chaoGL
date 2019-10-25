#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

#include <Render/Shader.h>
#include <Render/Texture.h>
#include <vector>

class Material : public Shader
{
public:

	Material() : Shader()
	{
		this->shader = nullptr;
	}

	Material(Shader* shader) : Shader(shader->vsPath, shader->fsPath)
	{
		this->shader = shader;
	}

	Material(string vsPath, string fsPath) : Shader(vsPath, fsPath)
	{
		//Shader(vsPath, fsPath);
		this->shader = dynamic_cast<Shader*>(this);
	}
	
	~Material()
	{
		
		delete this;
	}


	// texture
	void AddTexture(Texture &texture)
	{
		Shader::use();

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
			cout << "Reached to the max texture count for material " << fsPath.c_str() << endl;
			return;
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
				break;
		}
	}

	void ReplaceTexture(Texture& texture, TextureSlot slot)
	{
		Shader::use();
		textures[slot] = texture;
		SetParam(textures[slot].keyword, slot);
	}
	void use() override
	{
		Shader::use();
		BindTextures();
	}

	// set uniform values
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

public:

	static const int maxTextureCount = 16;

private:

	Shader* shader = nullptr;
	Texture textures[maxTextureCount];
	//unsigned int textureSlots[maxTextureCount];

	//vector<Texture> textures;
	//vector<unsigned int> textureSlots;
};

#endif // !MATERIAL_H



