#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

#include <Render/Shader.h>
#include <Render/Texture.h>
#include <vector>

class Material : public Shader
{
public:

	Material(Shader* shader) : Shader(shader->vsPath, shader->fsPath)
	{
		this->shader = shader;
	}

	Material(string vsPath, string fsPath) : Shader(vsPath, fsPath)
	{
		this->shader = dynamic_cast<Shader*>(this);
	}
	
	~Material()
	{
		delete this;
	}


	// texture
	void AddTexture(Texture texture)
	{
		Shader::use();

		int textureSlot = -1;
		for (int i = 0; i < maxTextureCount; i++)
		{
			if (textures[i].id == 0)
			{
				textureSlot = i;
				textures[textureSlot] = texture;
				break;
			}
		}
		if (textureSlot < 0)
		{
			cout << "Reached to the max texture count for material " << fsPath.c_str() << endl;
			return;
		}

		switch (texture.type)
		{
		default:
			break;
		case TextureType::Albedo:
			SetParam("material.albedo", textureSlot);
		case TextureType::Normal:
			SetParam("material.normal", textureSlot);
		case TextureType::Metallic:
			SetParam("material.metallic", textureSlot);
		case TextureType::Roughness:
			SetParam("material.roughness", textureSlot);
		case TextureType::MRO:
			SetParam("material.mro", textureSlot);
		case TextureType::AO:
			SetParam("material.ao", textureSlot);
		case TextureType::Specular:
			SetParam("material.specular", textureSlot);
		case TextureType::Diffuse:
			SetParam("material.diffuse", textureSlot);
		case TextureType::Cube:
			SetParam("environmentMap", textureSlot);
		case TextureType::Equirectangular:
			SetParam("equirectangularMap", textureSlot);

		}
	}

	void BindTextures()
	{
		for (int i = 0; i < maxTextureCount; i++)
		{
			if (textures[i].id != 0)
			{
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, textures[i].id);
				if(textures[i].useMip)
					glGenerateMipmap(GL_TEXTURE_2D);
			}
		}
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

	Shader* shader;
	Texture textures[maxTextureCount];
	//unsigned int textureSlots[maxTextureCount];

	//vector<Texture> textures;
	//vector<unsigned int> textureSlots;
};

#endif // !MATERIAL_H



