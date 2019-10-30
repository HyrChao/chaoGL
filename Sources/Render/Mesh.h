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
    // func
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, mat4 modelMat)
    {
        this->vertices = vertices;
        this->indices = indices;
		this -> modelMat = modelMat;

		this->material = make_unique<Material>(CommonAssets::instance->standardPBRShader);

		for (int i = 0; i < textures.size(); i++)
		{
			material->AddTexture(textures[i]);
		}
		material->SetModelMat(modelMat);

        SetupMesh();
    }
  //  ~Mesh()
  //  {
		//// no need to manually delete material, because we already use unique ptr
  //  }
    void Draw();

private:
    // render buffer
    unsigned int VAO, VBO, EBO;
	mat4 modelMat;
	// provent error during destroction
	unique_ptr<Material> material;
	 
	
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
