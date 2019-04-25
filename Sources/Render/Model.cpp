//
//  Model.cpp
//  chaoGL
//
//  Created by Chao on 2019/4/16.
//

#include "Model.h"

void Model::LoadModel(string path)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
        return;
    }
    
    directory = path.substr(0,path.find_last_of('/'));
    
    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode *node, const aiScene *scene)
{
    // process all mesh in node
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh, scene));
        
    }
    
    // process all child node
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
    
}

Mesh Model::ProcessMesh(aiMesh *mesh, const aiScene *scene)
{
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        glm::vec3 posVector;
        posVector.x = mesh->mVertices[i].x;
        posVector.y = mesh->mVertices[i].y;
        posVector.z = mesh->mVertices[i].z;
        vertex.Position = posVector;
        
        glm::vec3 norVector;
        norVector.x = mesh->mNormals[i].x;
        norVector.y = mesh->mNormals[i].y;
        norVector.z = mesh->mNormals[i].z;
        vertex.Normal = norVector;
        
        if(mesh->mTextureCoords[0]) // check if mesh have textureCoord0 information
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        
        vertices.push_back(vertex);
    }
    // process index
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    
    // process material
    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        vector<Texture> diffuseMaps = LoadMaterialTextures(material,
                                                           aiTextureType_DIFFUSE, TextureType::Diffuse);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        vector<Texture> specularMaps = LoadMaterialTextures(material,
                                                            aiTextureType_SPECULAR, TextureType::Specular);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
    
    return Mesh(vertices, indices, textures);
}

vector<Texture> Model::LoadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                 TextureType typeName)
{
    vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for(unsigned int j = 0; j < coRender::textures_loaded.size(); j++)
        {
            if(std::strcmp(coRender::textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(coRender::textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if(!skip)
        {
            Texture texture;
            texture.id = coRender::TextureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            coRender::textures_loaded.push_back(texture);
        }
        
    }
    return textures;
}

void Model::Draw(Shader* shader)
{
    for (unsigned int i = 9; i < meshes.size(); i++) {
        meshes[i].Draw(shader);
    }
}
