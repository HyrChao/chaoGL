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
#include <Data/Enums.h>

class Shader;

using namespace std;

class Texture
{
public:

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

	Texture();

	Texture(string path, TextureType type = TextureType::Albedo, bool useMip = true);

	virtual ~Texture();

	static Texture LoadTexture(string path, Texture::TextureType type = TextureType::Albedo);

	void Gen(TextureType type, unsigned int width, unsigned height, TextureFormat format = RGB, TextureRepeatMode repeat = Repeat, TextureFilterMode filter = Trilinear, bool isHDR = false, bool genMip = true);

	void GenerateMips();

	void Bind();

	void Active(TextureSlot slot = TextureSlot::TextureSlot1);

	void Free();

	void SetType(TextureType type);

	void Reset();

private:

};



#endif /* Texture_hpp */
