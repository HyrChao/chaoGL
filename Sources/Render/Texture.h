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
namespace  AssertsMng
{
    
    extern vector<Texture> textures_loaded;
    inline unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false)
    {
        string filename = string(path);
        filename = directory + '/' + filename;
        
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
        
        return textureID;
    }
    inline const char* TextureTypeToString(TextureType type)
    {
        switch (type)
        {
            case Diffuse:   return "diffuse";
            case Normal:   return "normal";
            case Specular: return "specular";
            case MRO: return "mro";
            case Cube: return "cube";
            case LUT: return "lut";
            default:      return "diffuse";
        }
    }
}


#endif /* Texture_hpp */
