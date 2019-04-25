//
//  Model.hpp
//  chaoGL
//
//  Created by Chao on 2019/4/16.
//

#ifndef Model_h
#define Model_h

#include <Render/Mesh.h>

class Model
{
public:
    
    Model(char *path)
    {
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
