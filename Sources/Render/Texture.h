//
//  Texture.hpp
//  IrrXML
//
//  Created by Chao on 2019/4/24.
//

#ifndef Texture_h
#define Texture_h

#include <glad/glad.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <stb_image.h>
#include <File/filesystem.h>

using namespace std;


enum TextureType
{
	Albedo,
    Normal,
	Metallic,
	Roughness,
	AO,
    MRO,
    Diffuse,
    Specular,
    Cube,
	Irridiance,
	PrefilterEnv,
	Equirectangular,
	LUT,
	BRDFLUT,
};

enum TextureFilterMode
{
	Point,
	Bilinear,
	Trilinear
};

enum TextureRepeatMode
{
	Repeat,
	Clamp
};

enum TextureFormat
{
	R,
	RG,
	RGB,
	RGBA,
};

enum TextureSlot
{
	TextureSlot1 =  0,
	TextureSlot2 =  1,
	TextureSlot3 =  3,
	TextureSlot4 =  4,
	TextureSlot5 =  5,
	TextureSlot6 =  6,
	TextureSlot7 =  7,
	TextureSlot8 =  8,
	TextureSlot9 =  9,
	TextureSlot10 = 10,
	TextureSlot11 = 11,
	TextureSlot12 = 12,
	TextureSlot13 = 13,
	TextureSlot14 = 14,
	TextureSlot15 = 15,
	TextureSlot16 = 16,
	TempSlot1 = 17,
	TempSlot2 = 18,
	TempSlot3 = 19,
	TempSlot4 = 20
};

enum GL_TextureSlot
{
	GL_TextureSlot1 = GL_TEXTURE0 + 0,
	GL_TextureSlot2 = GL_TEXTURE0 + 1,
	GL_TextureSlot3 = GL_TEXTURE0 + 3,
	GL_TextureSlot4 = GL_TEXTURE0 + 4,
	GL_TextureSlot5 = GL_TEXTURE0 + 5,
	GL_TextureSlot6 = GL_TEXTURE0 + 6,
	GL_TextureSlot7 = GL_TEXTURE0 + 7,
	GL_TextureSlot8 = GL_TEXTURE0 + 8,
	GL_TextureSlot9 = GL_TEXTURE0 + 9,
	GL_TextureSlot10 = GL_TEXTURE0 + 10,
	GL_TextureSlot11 = GL_TEXTURE0 + 11,
	GL_TextureSlot12 = GL_TEXTURE0 + 12,
	GL_TextureSlot13 = GL_TEXTURE0 + 13,
	GL_TextureSlot14 = GL_TEXTURE0 + 14,
	GL_TextureSlot15 = GL_TEXTURE0 + 15,
	GL_TextureSlot16 = GL_TEXTURE0 + 16,
	GL_TempSlot1 = GL_TEXTURE0 + 17,
	GL_TempSlot2 = GL_TEXTURE0 + 18,
	GL_TempSlot3 = GL_TEXTURE0 + 19,
	GL_TempSlot4 = GL_TEXTURE0 + 20
};

struct Texture
{
public:
    unsigned int id = 0;
    TextureType type = TextureType::Albedo;
	string keyword;
	TextureFormat format = TextureFormat::RGBA;
	TextureFilterMode fiterMode = Trilinear;
	unsigned int width = 8;
	unsigned int height = 8;
    string path = "/Assets/Texture/white.png";

	bool isCube = false;
	bool hasMip = false;
	bool loaded = false;
private:

	unsigned int gl_textureType = GL_TEXTURE_2D;

public:

	Texture()
	{
		id = 0;
		type = TextureType::Albedo;
		path = "/Assets/Texture/white.png";
	}

	Texture(string path, TextureType type, bool useMip)
	{
		this->type = type;
		this->path = path;
	}

	void Gen(TextureType type, unsigned int width, unsigned height, TextureFormat format = RGB,TextureRepeatMode repeat = Repeat, TextureFilterMode filter = Trilinear, bool isHDR = false, bool genMip = true)
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

	void GenerateMips()
	{
		if (!hasMip)
		{
			// Generate mipmap
			glBindTexture(gl_textureType, id);
			glGenerateMipmap(gl_textureType);
			hasMip = true;
		}
	}

	void Bind()
	{
		glBindTexture(gl_textureType, id);
	}

	void Active(TextureSlot slot = TextureSlot::TextureSlot1)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
	}

	void SetType(TextureType type) 
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



	void Reset()
	{
		id = 0;
		type = TextureType::Albedo;
		path = "/Assets/Texture/white.png";
	}

	inline void TextureFromFile(const char *path, bool gamma = false)
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
};



#endif /* Texture_hpp */
