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
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, glm::mat4 modelMat)
    {
        this->vertices = vertices;
        this->indices = indices;
		this -> modelMat = modelMat;

		//this->material = make_unique<Material>(CommonAssets::instance->standardPBRShader);

        SetupMesh();
    }

	void Draw(Material* material);
	void Draw(Material * material, glm::mat4 modelMat);

private:
    // render buffer
    unsigned int VAO, VBO, EBO;
	glm::mat4 modelMat;
	// provent error during destroction
	//unique_ptr<Material> material;
	 
	
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
