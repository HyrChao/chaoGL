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
	Equirectangular,
    LUT,
};

struct Texture
{
    unsigned int id = 0;
    TextureType type = TextureType::Albedo;
    string path = "/Assets/Texture/white.png";
	bool useMip = true;

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
};

#endif /* Texture_hpp */
