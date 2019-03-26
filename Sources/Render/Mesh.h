//
//  Mesh.hpp
//  chaoGL
//
//  Created by Chao on 2019/3/26.
//

#ifndef Mesh_h
#define Mesh_h

#include <chaoGL.h>
#include <Render/Shader.h
#include <string.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
}

struct Texture
{
    unsigned int id;
    TextureType type;
}

enum TextureType
{
    Diffuse,
    Normal,
    Specular,
    MRO,
    Cube,
    LUT,
}

class Mesh
{
public:
    // mesh data
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    // func
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
    ~Mesh();
    void Draw(Shader* shader);
private:
    // render buffer
    unsigned int VAO, VBO, EBO;
    // func
    void SetupMesh();
    
    inline const char* TextureTypeToString(TextureType type)
    {
        switch (type)
        {
            case Diffuse:   return "Diffuse";
            case Normal:   return "Normal";
            case Specular: return "Specular";
            case MRO: return "MRO";
            case Cube: return "Cube";
            case LUT: return "LUT";
            default:      return "Diffuse";
        }
    }
}
#endif /* Mesh_h */