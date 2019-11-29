//
//  Texture.cpp
//  IrrXML
//
//  Created by Chao on 2019/4/24.
//

#include "Texture.h"

Texture::Texture()
{
	id = 0;
	type = TextureType::Albedo;
	path = "/Assets/Texture/white.png";
}

Texture::Texture(string path, TextureType type, bool useMip)
{
	this->type = type;
	this->path = path;
}

Texture::~Texture()
{
	//Free();
}

void Texture::Gen(TextureType type, unsigned int width, unsigned height, TextureFormat format, TextureRepeatMode repeat, TextureFilterMode filter, bool isHDR, bool genMip)
{
	if (!loaded)
	{
		unsigned int gl_format = GL_RGBA;
		unsigned int gl_internalFormat = GL_RGBA;
		unsigned int gl_repeatMode = GL_REPEAT;

		this->height = height;
		this->width = width;
		this->format = format;
		this->fiterMode = filter;

		switch (format)
		{
		case R:
			gl_format = GL_R;
			gl_internalFormat = GL_R;
			break;
		case RG:
			gl_format = GL_RG;
			gl_internalFormat = GL_RG;
			break;
		case RGB:
			gl_format = GL_RGB;
			gl_internalFormat = GL_RGB;
			break;
		case RGBA:
			gl_format = GL_RGBA;
			gl_internalFormat = GL_RGBA;
			break;
		default:
			gl_format = GL_RGB;
			gl_internalFormat = GL_RGB;
			break;
		}

		if (isHDR)
		{
			switch (format)
			{
			case R:
				gl_internalFormat = GL_R16F;
				break;
			case RG:
				gl_internalFormat = GL_RG16F;
				break;
			case RGB:
				gl_internalFormat = GL_RGB16F;
				break;
			case RGBA:
				gl_internalFormat = GL_RGBA16F;
				break;
			default:
				gl_internalFormat = GL_RGB16F;
				break;
			}
		}

		switch (repeat)
		{
		case Repeat:
			gl_repeatMode = GL_REPEAT;
			break;
		case Clamp:
			gl_repeatMode = GL_CLAMP_TO_EDGE;
			break;
		default:
			gl_repeatMode = GL_REPEAT;
			break;
		}

		SetType(type);

		// Generate texture
		glGenTextures(1, &id);

		if (isCube)
		{
			gl_textureType = GL_TEXTURE_CUBE_MAP;
			glBindTexture(gl_textureType, id);
			for (int i = 0; i < 6; i++)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, gl_internalFormat, width, height, 0, gl_format, GL_FLOAT, 0);
			}
		}
		else
		{
			gl_textureType = GL_TEXTURE_2D;
			glBindTexture(gl_textureType, id);
			glTexImage2D(GL_TEXTURE_2D, 0, gl_internalFormat, width, height, 0, gl_format, GL_FLOAT, 0);

		}
		// Set texture parameters 
		glTexParameteri(gl_textureType, GL_TEXTURE_WRAP_S, gl_repeatMode);
		glTexParameteri(gl_textureType, GL_TEXTURE_WRAP_T, gl_repeatMode);
		glTexParameteri(gl_textureType, GL_TEXTURE_WRAP_R, gl_repeatMode);
		glTexParameteri(gl_textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (genMip && !hasMip)
		{
			glGenerateMipmap(gl_textureType);
			hasMip = true;
		}

		if (filter == Trilinear)
		{
			glTexParameteri(gl_textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}
		else
		{
			glTexParameteri(gl_textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}


		loaded = true;
	}
}

void Texture::GenerateMips()
{
	if (!hasMip)
	{
		// Generate mipmap
		glBindTexture(gl_textureType, id);
		glGenerateMipmap(gl_textureType);
		hasMip = true;
	}
}

void Texture::Bind()
{
	glBindTexture(gl_textureType, id);
}

void Texture::Active(TextureSlot slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
}

void Texture::Free()
{
	if(id)
		glDeleteTextures(1, &id);
}

void Texture::SetType(TextureType type)
{
	this->type = type;
	switch (type)
	{
	default:
		break;
	case TextureType::Albedo:
		this->keyword = "material.albedo";
		break;
	case TextureType::Normal:
		this->keyword = "material.normal";
		break;
	case TextureType::Metallic:
		this->keyword = "material.metallic";
		break;
	case TextureType::Roughness:
		this->keyword = "material.roughness";
		break;
	case TextureType::MRO:
		this->keyword = "material.mro";
		break;
	case TextureType::AO:
		this->keyword = "material.ao";
		break;
	case TextureType::Specular:
		this->keyword = "material.specular";
		break;
	case TextureType::Diffuse:
		this->keyword = "material.diffuse";
		break;
	case TextureType::Cube:
		this->keyword = "environmentMap";
		isCube = true;
		break;
	case TextureType::Irridiance:
		this->keyword = "IBL.irradianceMap";
		isCube = true;
		break;
	case TextureType::PrefilterEnv:
		this->keyword = "IBL.prefilterEnv";
		isCube = true;
		break;
	case TextureType::Equirectangular:
		this->keyword = "equirectangularMap";
		isCube = true;
		break;
	case TextureType::BRDFLUT:
		this->keyword = "IBL.BRDFPrefilterMap";
		break;
	}
}

void Texture::Reset()
{
	id = 0;
	type = TextureType::Albedo;
	path = "/Assets/Texture/white.png";
}

void Texture::TextureFromFile(const char * path, bool gamma)
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
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	this->id = textureID;
}
