
#pragma once

#ifndef ASSETSMANAGER_H
#define ASSETSMANAGER_H

#include<Render/Texture.h>

class  AssetsManager
{

public:

	static vector<Texture> textures_loaded;
	
public:

	inline static unsigned int TextureFromFile(const char *path, bool gamma = false)
	{
		string filename = string(FileSystem::getPath(path).c_str());

		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;
			else
				format = GL_RGBA;
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << filename << std::endl;
			stbi_image_free(data);
		}

		return textureID;
	}

	inline static unsigned int TextureFromFile_FullPath(const char *fullpath, bool gamma = false)
	{
		string filename = string(fullpath);

		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;
			else
				format = GL_RGBA;
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << filename << std::endl;
			stbi_image_free(data);
		}

		return textureID;
	}

	inline static unsigned int CubeTextureFromFile(const char* path)
	{

		string filePath = string(FileSystem::getPath(path).c_str());

		unsigned int hdrTextureID;

		stbi_set_flip_vertically_on_load(true);

		int width, height, nrComponents;
		float *data = stbi_loadf(filePath.c_str(), &width, &height, &nrComponents, 0);
		unsigned int hdrTexture;
		if (data)
		{
			glGenTextures(1, &hdrTextureID);
			glBindTexture(GL_TEXTURE_2D, hdrTextureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load HDR image." << std::endl;
		}

		return hdrTextureID;
	}

	inline static string ExtractFileName(const std::string& fullPath)
	{
		const size_t lastSlashIndex = fullPath.find_last_of("/\\");
		return fullPath.substr(lastSlashIndex + 1);
	}

	inline static const char* TextureTypeToString(Texture::TextureType type)
	{
		switch (type)
		{
		case Texture::Diffuse:   return "_diffuse";
		case Texture::Albedo: return "_albedo";
		case Texture::Metallic: return "_matallic";
		case Texture::Roughness: return "_roughness";
		case Texture::Normal:   return "_normal";
		case Texture::Specular: return "_specular";
		case Texture::MRO: return "_mro";
		case Texture::Cube: return "_cube";
		case Texture::LUT: return "_lut";
		case Texture::AO: return "_ao";
		default:      return "_diffuse";
		}
	}
};

#endif // !ASSETSMANAGER_H