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
				break;
			}
		}
		if (m_textureSlot < 0)
		{
			cout << "Reached to the max texture count for material " << fsPath.c_str() << endl;
			return;
		}

		if (texture.genMip && !texture.loaded)
		{
			// Set texture parameters 
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// Generate mipmap
			glBindTexture(texture.format, texture.id);
			glGenerateMipmap(texture.format);

			texture.loaded = true;
		}

		switch (texture.type)
		{
		default:
			break;
		case TextureType::Albedo:
			SetParam("material.albedo", m_textureSlot);
			break;
		case TextureType::Normal:
			SetParam("material.normal", m_textureSlot);
			break;
		case TextureType::Metallic:
			SetParam("material.metallic", m_textureSlot);
			break;
		case TextureType::Roughness:
			SetParam("material.roughness", m_textureSlot);
			break;
		case TextureType::MRO:
			SetParam("material.mro", m_textureSlot);
			break;
		case TextureType::AO:
			SetParam("material.ao", m_textureSlot);
			break;
		case TextureType::Specular:
			SetParam("material.specular", m_textureSlot);
			break;
		case TextureType::Diffuse:
			SetParam("material.diffuse", m_textureSlot);
			break;
		case TextureType::Cube:
			SetParam("environmentMap", m_textureSlot);
			break;
		case TextureType::Irridiance:
			SetParam("IBL.irradianceMap", m_textureSlot);
			break;	
		case TextureType::PrefilterEnv:
			SetParam("IBL.prefilterEnv", m_textureSlot);
			break;
		case TextureType::Equirectangular:
			SetParam("equirectangularMap", m_textureSlot);
			break;
		case TextureType::BRDFLUT:
			SetParam("IBL.BRDFPrefilterMap", m_textureSlot);
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
				glBindTexture(textures[i].format, textures[i].id);
			}
			else
				break;
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

	Shader* shader = nullptr;
	Texture textures[maxTextureCount];
	//unsigned int textureSlots[maxTextureCount];

	//vector<Texture> textures;
	//vector<unsigned int> textureSlots;
};

#endif // !MATERIAL_H



