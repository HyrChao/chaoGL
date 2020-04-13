//
//  Model.hpp
//  chaoGL
//
//  Created by Chao on 2019/4/16.
//

#ifndef Model_h
#define Model_h

#include <Render/Mesh.h>
#include <filesystem>
#include <iostream>
#include <Object/UniqueID.h>

class Model : public Transform, public UniqueID<Model>
{

public:
    
	Model() = delete;
	Model(string const &path, glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale, bool isDynamic, bool gamma);
	~Model();

	void Draw(Material * material);
	void Bind(Material * material);

	void ChangeModelMat(glm::mat4 &mat);
	static void UpdateModelMatsInList();

private:

	// update model mat
	virtual void UpdateModelMat();

	void LoadModel(string path);

public:

	bool isDynamic = false;

protected:

	glm::mat4 modelMat;

private:
    // model data
    vector<Mesh> meshes;
    string directory;
	string name;

	bool gammaCorrection;

	static std::map<unsigned int, Model*> modelList;

    // functions
	void ProcessTextures(string path);
	void LoadDefaultTexture(vector<Texture>& textures, Texture::TextureType type);
    void ProcessNode(aiNode *node, const aiScene *scene);
    Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
    vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type,
		Texture::TextureType typeName);
	Texture Model::LoadTexture(string path, Texture::TextureType type);

};

#endif
