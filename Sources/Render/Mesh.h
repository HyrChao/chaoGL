//
//  Mesh.hpp
//  chaoGL
//
//  Created by Chao on 2019/3/26.
//

#ifndef Mesh_h
#define Mesh_h

#include <chaoGL.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
//#include <assimp/Importer.hpp>
//#include <assimp/postprocess.h>
//#include <assimp/scene.h>

using namespace std;

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};


class Mesh
{
public:
    // mesh data
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    // func
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        SetupMesh();
    }
    ~Mesh()
    {
        
    }
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
            case Diffuse:   return "diffuse";
            case Normal:   return "normal";
            case Specular: return "specular";
            case MRO: return "mro";
            case Cube: return "cube";
            case LUT: return "lut";
            default:      return "diffuse";
        }
    }
};
#endif /* Mesh_h */
