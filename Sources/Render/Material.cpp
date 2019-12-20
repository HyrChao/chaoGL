#include <Render/Material.h>
#include <filesystem>
#include <fstream>
#include <iostream>
// set uniform values


// Remove texture from material's texture list, do not call it in main loop


// texture

Material::Material()
{
	this->shader = CommonAssets::instance->defaltErrorShader;
	isShaderInstance = true;
}

Material::Material(Shader * shader)
{
	this->shader = shader;
	isShaderInstance = true;
}

Material::Material(Shader * shader, string texturePath)
{
	this->shader = shader;
	isShaderInstance = true;

	vector<Texture> textures;
	// process textures
	for (const auto & entry : std::filesystem::directory_iterator(FileSystem::getPath(texturePath)))
	{
		// check all files in directory
		string texturePath = entry.path().string();
		string textureName = AssetsManager::ExtractFileName(texturePath);
		if (textureName.find(AssetsManager::TextureTypeToString(Texture::TextureType::Albedo)) != string::npos)
		{
			Texture textureLoaded = LoadTexture(texturePath, Texture::TextureType::Albedo);
			textures.push_back(textureLoaded);
			std::cout << "Load texture " << textureName << " in path " << texturePath << " for material " << (int)this << std::endl;
		}

		if (textureName.find(AssetsManager::TextureTypeToString(Texture::TextureType::Normal)) != string::npos)
		{
			Texture textureLoaded = LoadTexture(texturePath, Texture::TextureType::Normal);
			textures.push_back(textureLoaded);
			std::cout << "Load texture " << textureName << " in path " << texturePath << " for material " << (int)this << std::endl;
		}

		if (textureName.find(AssetsManager::TextureTypeToString(Texture::TextureType::Roughness)) != string::npos)
		{
			Texture textureLoaded = LoadTexture(texturePath, Texture::TextureType::Roughness);
			textures.push_back(textureLoaded);
			std::cout << "Load texture " << textureName << " in path " << texturePath << " for material " << (int)this << std::endl;
		}

		if (textureName.find(AssetsManager::TextureTypeToString(Texture::TextureType::Metallic)) != string::npos)
		{
			Texture textureLoaded = LoadTexture(texturePath, Texture::TextureType::Metallic);
			textures.push_back(textureLoaded);
			std::cout << "Load texture " << textureName << " in path " << texturePath << " for material " << (int)this << std::endl;
		}

		if (textureName.find(AssetsManager::TextureTypeToString(Texture::TextureType::MRO)) != string::npos)
		{
			Texture textureLoaded = LoadTexture(texturePath, Texture::TextureType::MRO);
			textures.push_back(textureLoaded);
			std::cout << "Load texture " << textureName << " in path " << texturePath << " for material " << (int)this << std::endl;
		}

		if (textureName.find(AssetsManager::TextureTypeToString(Texture::TextureType::AO)) != string::npos)
		{
			Texture textureLoaded = LoadTexture(texturePath, Texture::TextureType::AO);
			textures.push_back(textureLoaded);
			std::cout << "Load texture " << textureName << " in path " << texturePath << " for material " << (int)this << std::endl;
		}
	}

	LoadDefaultTexture(textures, Texture::Albedo);
	LoadDefaultTexture(textures, Texture::Normal);
	LoadDefaultTexture(textures, Texture::Roughness);
	LoadDefaultTexture(textures, Texture::Metallic);
	LoadDefaultTexture(textures, Texture::AO);

	for (auto it = textures.begin(); it != textures.end(); it++)
	{
		Texture tex = *it;
		this->AddTexture(tex);
	}

}

Material::Material(string vsPath, string fsPath)
{
	this->shader_unique = make_unique<Shader>(vsPath, fsPath);
	this->shader = shader_unique.get();
	isShaderInstance = false;
}

void Material::SetModelMat(glm::mat4 mat)
{
	mat4Params["model"] = mat;
}

void Material::AddTexture(Texture & texture)
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
		if (textures[i].id == texture.id && textures[i].type == texture.type)
		{
			break;
		}
	}
	if (m_textureSlot < 0)
	{
		cout << "Reached to the max texture count for material " << shader->fsPath.c_str() << endl;
		return;
	}

}

void Material::ClearTextrues()
{
	for (int i = 0; i < maxTextureCount; i++)
	{
		textures[i].Reset();
	}
}

void Material::RemoveTexture(Texture & texture)
{
	for (int i = 0; i < maxTextureCount; i++)
	{
		if (textures[i].id == texture.id)
			textures[i].Reset();
	}
}

void Material::BindTextures()
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

void Material::ReplaceTexture(Texture & texture)
{
	for (int i = 0; i < maxTextureCount; i++)
	{
		if (textures[i].type == texture.type)
		{
			textures[i] = texture;
			return;
		}
	}

	AddTexture(texture);

}

void Material::ReplaceTexture(Texture & pre_texture, Texture & texture)
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

void Material::use()
{
	this->shader->use();
	UpdateParams();
	BindTextures();
}

void Material::ChangeShader(Shader * shader)
{
	this->shader = shader;
}

Shader * Material::GetShader()
{
	return shader;
}

void Material::SetParam(const string & name, bool value)
{
	boolParams[name] = value;
	//boolParams.insert(std::pair<string, bool>(name, value));
}

void Material::SetParam(const string & name, int value)
{
	intParams[name] = value;
}

void Material::SetParam(const string & name, float value)
{
	floatParams[name] = value;
}

void Material::SetParam(const string & name, float x, float y, float z)
{
	vec3Params[name] = glm::vec3(x, y, z);
}

void Material::SetParam(const string & name, glm::vec3& vec3)
{
	vec3Params[name] = vec3;
}

void Material::SetParam(const string & name, float x, float y, float z, float w)
{
	vec4Params[name] = glm::vec4(x, y, z, w);
}

void Material::SetParam(const string & name, glm::vec4& vec4)
{
	vec4Params[name] = vec4;
}

void Material::SetParam(const string & name, glm::mat4& mat)
{
	mat4Params[name] = mat;
}

void Material::UpdateParams()
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
	for (map<string, glm::vec2>::iterator it = vec2Params.begin(); it != vec2Params.end(); it++)
	{
		shader->setVec2f(it->first, it->second);
	}
	for (map<string, glm::vec3>::iterator it = vec3Params.begin(); it != vec3Params.end(); it++)
	{
		shader->setVec3f(it->first, it->second);
	}
	for (map<string, glm::vec4>::iterator it = vec4Params.begin(); it != vec4Params.end(); it++)
	{
		shader->setVec4f(it->first, it->second);
	}
	for (map<string, glm::mat4>::iterator it = mat4Params.begin(); it != mat4Params.end(); it++)
	{
		shader->setMat4f(it->first, it->second);
	}
}

void Material::LoadDefaultTexture(vector<Texture>& textures, Texture::TextureType type)
{
	bool hasTex = false;
	for (auto i = textures.begin(); i != textures.end(); i++)
	{
		Texture tex = *i;
		if (tex.type == type)
		{
			hasTex = true;
			break;
		}

	}

	if (!hasTex)
	{
		Texture texture;
		texture.SetType(type);
		if (type == Texture::AO || type == Texture::Albedo || type == Texture::Roughness)
			texture.id = CommonAssets::instance->whiteTex;
		else if (type == Texture::Normal)
			texture.id = CommonAssets::instance->flatNormal;
		else
			texture.id = CommonAssets::instance->blackTex;

		textures.push_back(texture);
	}

}

Texture Material::LoadTexture(string path, Texture::TextureType type)
{
	Texture texture;
	bool skip = false;
	for (unsigned int j = 0; j < AssetsManager::textures_loaded.size(); j++)
	{
		if (std::strcmp(AssetsManager::textures_loaded[j].path.data(), path.c_str()) == 0)
		{
			texture = AssetsManager::textures_loaded[j];
			skip = true;
			break;
		}
	}
	if (!skip)
	{
		texture.id = AssetsManager::TextureFromFile_FullPath(path.c_str());
		texture.SetType(type);
		texture.path = path.c_str();
		AssetsManager::textures_loaded.push_back(texture);
	}
	return texture;
}
