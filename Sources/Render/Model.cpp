//
//  Model.cpp
//  chaoGL
//
//  Created by Chao on 2019/4/16.
//

#include "Model.h"


// update model mat

inline void Model::UpdateMat()
{
	glm::mat4 newModelMat = glm::mat4(1.0f);
	newModelMat = glm::translate(newModelMat, this->pos);
	newModelMat = glm::rotate(newModelMat, this->rotation.x, glm::vec3(1.0, 0, 0));
	newModelMat = glm::rotate(newModelMat, this->rotation.y, glm::vec3(0, 1.0, 0));
	newModelMat = glm::rotate(newModelMat, this->rotation.z, glm::vec3(0, 0, 1.0));
	newModelMat = glm::scale(newModelMat, this->scale);

	modelMat = newModelMat;
}

void Model::LoadModel_SingleMaterial(string path)
{
    Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	//const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_FixInfacingNormals);
    
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
        return;
    }
    
	name = AssetsManager::ExtractFileName(path);
    directory = path.substr(0, path.find_last_of("/"));
	cout << "Loading model "<< name << " from path "<< directory << importer.GetErrorString() << endl;
    ProcessNode(scene->mRootNode, scene);
	cout << "Load model " << name << "success" << endl;
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

		if (mesh->mTangents) // check if mesh have tangent information
		{
			glm::vec3 vec;
			vec.x = mesh->mTangents[i].x;
			vec.y = mesh->mTangents[i].y;
			vec.z = mesh->mTangents[i].z;
			vertex.Tangent = vec;
		}
		else
			vertex.Tangent = glm::vec3(0.0f, 0.0f, 0.0f);
        
		if (mesh->mBitangents) // check if mesh have bitangent information
		{
			glm::vec3 vec;
			vec.x = mesh->mBitangents[i].x;
			vec.y = mesh->mBitangents[i].y;
			vec.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vec;
		}
		else
			vertex.Bitangent = glm::vec3(0.0f, 0.0f, 0.0f);

		if (mesh->mColors[0]) // check if mesh have color information
		{
			glm::vec4 vec;
			vec.r = mesh->mColors[0][i].r;
			vec.g = mesh->mColors[0][i].g;
			vec.b = mesh->mColors[0][i].b;
			vec.a = mesh->mColors[0][i].a;
			vertex.Color = vec;
		}
		else
			vertex.Color = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);



        vertices.push_back(vertex);
    }
    // process index
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    return Mesh(vertices, indices, this->modelMat);
}

void Model::ProcessTextures()
{

}



//vector<Texture> Model::LoadMaterialTextures(aiMaterial *mat, aiTextureType type,
//                                 TextureType typeName)
//{
//    vector<Texture> textures;
//    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
//    {
//        aiString str;
//        mat->GetTexture(type, i, &str);
//        bool skip = false;
//        for(unsigned int j = 0; j < AssetsManager::textures_loaded.size(); j++)
//        {
//            if(std::strcmp(AssetsManager::textures_loaded[j].path.data(), str.C_Str()) == 0)
//            {
//                textures.push_back(AssetsManager::textures_loaded[j]);
//                skip = true;
//                break;
//            }
//        }
//        if(!skip)
//        {
//            Texture texture;
//            texture.id = AssetsManager::TextureFromFile(str.C_Str(), directory);
//            texture.type = typeName;
//            texture.path = str.C_Str();
//            textures.push_back(texture);
//            AssetsManager::textures_loaded.push_back(texture);
//        }
//        
//    }
//    return textures;
//}

Model::Model(string const & path, bool gamma, glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale) : Transform(pos, rotation, scale)
{
	string fullPath = FileSystem::getPath(path);
	modelMat = glm::translate(modelMat, this->pos);
	modelMat = glm::rotate(modelMat, this->rotation.x, glm::vec3(1.0, 0, 0));
	modelMat = glm::rotate(modelMat, this->rotation.y, glm::vec3(0, 1.0, 0));
	modelMat = glm::rotate(modelMat, this->rotation.z, glm::vec3(0, 0, 1.0));
	modelMat = glm::scale(modelMat, this->scale);
	gammaCorrection = gamma;
	LoadModel_SingleMaterial(fullPath);
}

void Model::Draw(Material* material)
{
    
    for (unsigned int i = 0; i < meshes.size(); i++) 
	{     
        meshes[i].Draw(material);
    }
}
void Model::Draw(Material* material, glm::mat4 modelMat)
{

	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(material, modelMat);
	}
}

inline void Model::SetPos(glm::vec3 pos)
{
	Transform::SetPos(pos);
	UpdateMat();
}

inline void Model::SetRotation(glm::vec3 rotation)
{
	Transform::SetRotation(rotation);
	UpdateMat();
}

inline void Model::SetScale(glm::vec3 scale)
{
	Transform::SetScale(scale);
	UpdateMat();
}

inline void Model::Translate(glm::vec3 translate)
{
	Transform::Translate(translate);
	UpdateMat();
}

inline void Model::Rotate(glm::vec3 rotate)
{
	Transform::Rotate(rotate);
	UpdateMat();
}

inline void Model::Scale(glm::vec3 scale)
{
	Transform::Scale(scale);
	UpdateMat();
}
