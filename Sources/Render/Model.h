//
//  Model.hpp
//  chaoGL
//
//  Created by Chao on 2019/4/16.
//

#ifndef Model_h
#define Model_h

#include <Render/Mesh.h>


class Model : public Transform
{
public:
    glm::mat4 modelMat;
    
public:
    
    bool gammaCorrection;
    
    Model(string const &path, bool gamma = false, glm::vec3 pos = glm::vec3(0),glm::vec3 rotation = glm::vec3(0),glm::vec3 scale = glm::vec3(1)) : Transform(pos,rotation,scale)
    {
        glm::mat4 model;
        modelMat = model;
        modelMat = glm::translate(modelMat, pos);
        modelMat = glm::rotate(modelMat,rotation.x,glm::vec3(1.0,0,0));
        modelMat = glm::rotate(modelMat,rotation.y,glm::vec3(0,1.0,0));
        modelMat = glm::rotate(modelMat,rotation.z,glm::vec3(0,0,1.0));
        //modelMat = glm::scale(modelMat, scale);
        gammaCorrection = gamma;
        LoadModel(path);
    }
    void Draw(Shader* shader);
    
private:
    // model data
    vector<Mesh> meshes;
    string directory;
    
    // functions
    void LoadModel(string path);
    void ProcessNode(aiNode *node, const aiScene *scene);
    Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
    vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                         TextureType typeName);
};

#endif /* Model_hpp */
