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
    
    Model(string const &path, bool gamma = false, glm::vec3 pos = glm::vec3(0),glm::vec3 rotation = glm::vec3(0),glm::vec3 scale = glm::vec3(1)) : Transform(pos,rotation,scale)
    {
		string fullPath = FileSystem::getPath(path);
        modelMat = glm::translate(modelMat, this->pos);
        modelMat = glm::rotate(modelMat, this->rotation.x,glm::vec3(1.0,0,0));
        modelMat = glm::rotate(modelMat,this->rotation.y,glm::vec3(0,1.0,0));
        modelMat = glm::rotate(modelMat,this->rotation.z,glm::vec3(0,0,1.0));
        modelMat = glm::scale(modelMat, this->scale);
        gammaCorrection = gamma;
		LoadModel_SingleMaterial(fullPath);
    }
    void Draw();

	glm::mat4 GetMat() { return modelMat; }

	virtual void SetPos(glm::vec3 pos)
	{
		Transform::SetPos(pos);
		UpdateMat();
	}
	virtual void SetRotation(glm::vec3 rotation)
	{
		Transform::SetRotation(rotation);
		UpdateMat();
	}
	virtual void SetScale(glm::vec3 scale)
	{
		Transform::SetScale(scale);
		UpdateMat();
	}
	virtual void Translate(glm::vec3 translate)
	{
		Transform::Translate(translate);
		UpdateMat();
	}

	virtual void Rotate(glm::vec3 rotate)
	{
		Transform::Rotate(rotate);
		UpdateMat();
	}
	virtual void Scale(glm::vec3 scale)
	{
		Transform::Scale(scale);
		UpdateMat();
	}
    
protected:
	glm::mat4 modelMat;

private:
    // model data
    vector<Mesh> meshes;
    string directory;
	string name;
	// update model mat
	void UpdateMat()
	{
		glm::mat4 newModelMat = glm::mat4(1.0f);
		newModelMat = glm::translate(newModelMat, this->pos);
		newModelMat = glm::rotate(newModelMat, this->rotation.x, glm::vec3(1.0, 0, 0));
		newModelMat = glm::rotate(newModelMat, this->rotation.y, glm::vec3(0, 1.0, 0));
		newModelMat = glm::rotate(newModelMat, this->rotation.z, glm::vec3(0, 0, 1.0));
		newModelMat = glm::scale(newModelMat, this->scale);

		modelMat = newModelMat;
	}

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
