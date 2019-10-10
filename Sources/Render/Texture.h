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
    Diffuse,
    Normal,
    Specular,
    MRO,
    Cube,
    LUT,
};

struct Texture
{
    unsigned int id;
    TextureType type;
    string path;
};

#endif /* Texture_hpp */
