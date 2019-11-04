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
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
	glm::vec4 Color;
};


class Mesh
{
public:
    // mesh data
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    // func
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, mat4 modelMat)
    {
        this->vertices = vertices;
        this->indices = indices;
		this -> modelMat = modelMat;

		this->material = make_unique<Material>(CommonAssets::instance->standardPBRShader);

		for (auto it = textures.begin(); it != textures.end(); it++)
		{
			Texture tex = *it;
			material->AddTexture(tex);
		}
		
		material->SetModelMat(modelMat);

        SetupMesh();
    }

    void Draw();

private:
    // render buffer
    unsigned int VAO, VBO, EBO;
	mat4 modelMat;
	// provent error during destroction
	unique_ptr<Material> material;
	 
	
    // func
    void SetupMesh();
    
    inline const char* TextureTypeToString(Texture::TextureType type)
    {
        switch (type)
        {
            case Texture::Diffuse:   return "diffuse";
            case Texture::Normal:   return "normal";
            case Texture::Specular: return "specular";
            case Texture::MRO: return "mro";
            case Texture::Cube: return "cube";
            case Texture::LUT: return "lut";
            default:      return "diffuse";
        }
    }
};
#endif /* Mesh_h */
