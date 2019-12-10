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


enum TextureSlot
{
	TextureSlot1 = 0,
	TextureSlot2 = 1,
	TextureSlot3 = 3,
	TextureSlot4 = 4,
	TextureSlot5 = 5,
	TextureSlot6 = 6,
	TextureSlot7 = 7,
	TextureSlot8 = 8,
	TextureSlot9 = 9,
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
	TempSlot4 = 20,
	PBR_BRDF = 21,
	PBR_Prefilter = 22,
	PBR_Irridiance = 23,
	Shadowmap = 24
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

	Texture(string path, TextureType type, bool useMip);

	virtual ~Texture();


	void Gen(TextureType type, unsigned int width, unsigned height, TextureFormat format = RGB, TextureRepeatMode repeat = Repeat, TextureFilterMode filter = Trilinear, bool isHDR = false, bool genMip = true);

	void GenerateMips();

	void Bind();

	void Active(TextureSlot slot = TextureSlot::TextureSlot1);

	void Free();

	void SetType(TextureType type);

	void Reset();

	inline void TextureFromFile(const char *path, bool gamma = false);

};



#endif /* Texture_hpp */
