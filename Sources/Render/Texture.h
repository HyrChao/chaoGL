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
	Equirectangular,
    LUT,
};

enum TextureSlot
{
	MaterialSlot1 = GL_TEXTURE0 + 0,
	MaterialSlot2 = GL_TEXTURE0 + 1,
	MaterialSlot3 = GL_TEXTURE0 + 3,
	MaterialSlot4 = GL_TEXTURE0 + 4,
	MaterialSlot5 = GL_TEXTURE0 + 5,
	MaterialSlot6 = GL_TEXTURE0 + 6,
	MaterialSlot7 = GL_TEXTURE0 + 7,
	MaterialSlot8 = GL_TEXTURE0 + 8,
	MaterialSlot9 = GL_TEXTURE0 + 9,
	MaterialSlot10 = GL_TEXTURE0 + 10,
	MaterialSlot11 = GL_TEXTURE0 + 11,
	MaterialSlot12 = GL_TEXTURE0 + 12,
	MaterialSlot13 = GL_TEXTURE0 + 13,
	MaterialSlot14 = GL_TEXTURE0 + 14,
	MaterialSlot15 = GL_TEXTURE0 + 15,
	MaterialSlot16 = GL_TEXTURE0 + 16,
	TempSlot1 = GL_TEXTURE0 + 17,
	TempSlot2 = GL_TEXTURE0 + 18,
	TempSlot3 = GL_TEXTURE0 + 19,
	TempSlot4 = GL_TEXTURE0 + 20
};

struct Texture
{
    unsigned int id = 0;
    TextureType type = TextureType::Albedo;
    string path = "/Assets/Texture/white.png";
	bool useMip = true;
	int format = GL_TEXTURE_2D;

	Texture()
	{
		id = 0;
		type = TextureType::Albedo;
		path = "/Assets/Texture/white.png";
		useMip = true;
	}

	Texture(string path, TextureType type, bool useMip)
	{
		this->type = type;
		this->path = path;
		this->useMip = useMip;
	}

	void SetType(TextureType type) 
	{
		this->type = type;
		if (type == TextureType::Cube)
			format = GL_TEXTURE_CUBE_MAP;
		else if (type == TextureType::Irridiance)
			format = GL_TEXTURE_CUBE_MAP;
	}
};



#endif /* Texture_hpp */
