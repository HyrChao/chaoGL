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

class Model : public Transform
{

public:
    
    bool gammaCorrection;
    
	Model(string const &path, bool gamma = false, glm::vec3 pos = glm::vec3(0), glm::vec3 rotation = glm::vec3(0), glm::vec3 scale = glm::vec3(1));
	void Draw(Material * material);

	void Draw(Material * material, glm::mat4 modelMat);

	glm::mat4 GetMat() { return modelMat; }

	virtual void SetPos(glm::vec3 pos);
	virtual void SetRotation(glm::vec3 rotation);
	virtual void SetScale(glm::vec3 scale);
	virtual void Translate(glm::vec3 translate);

	virtual void Rotate(glm::vec3 rotate);
	virtual void Scale(glm::vec3 scale);
    
protected:
	glm::mat4 modelMat;

private:
    // model data
    vector<Mesh> meshes;
    string directory;
	string name;
	// update model mat
	void UpdateMat();

    // functions
    void LoadModel_SingleMaterial(string path);
	void ProcessTextures();
	void LoadDefaultTexture(vector<Texture>& textures, Texture::TextureType type);
    void ProcessNode(aiNode *node, const aiScene *scene);
    Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
    vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type,
		Texture::TextureType typeName);
	Texture Model::LoadTexture(string path, Texture::TextureType type);

};

#endif /* Model_hpp */
