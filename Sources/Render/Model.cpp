//
//  Model.cpp
//  chaoGL
//
//  Created by Chao on 2019/4/16.
//

#include "Model.h"

std::map<unsigned int, Model*> Model::modelList;

Model::Model(string const &path, glm::vec3 pos = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f), bool isDynamic = true, bool gamma = false) 
	:
	Transform(pos, rotation, scale),
	isDynamic(isDynamic),
	gammaCorrection(gamma)

{
	string fullPath = FileSystem::getPath(path);
	LoadModel(fullPath);
	modelList[GetID()] = this;
}

Model::	~Model()
{
	modelList[GetID()] = nullptr;
}


void Model::LoadModel(string path)
{
    Assimp::Importer importer;
	float globalScale = 10.0f;
	importer.SetPropertyFloat(AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, globalScale);
	//const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_GlobalScale);
	//const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_FixInfacingNormals);
    
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
		cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
		cout << "Load model " << name << " failed!" << endl;
        return;
    }
    
	name = AssetsManager::ExtractFileName(path);
    directory = path.substr(0, path.find_last_of("/"));
	cout << "Loading model "<< name << " from path "<< directory << importer.GetErrorString() << endl;
    ProcessNode(scene->mRootNode, scene);
	cout << "Load model " << name << " success" << endl;
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

    return Mesh(vertices, indices, &modelMat);
}


void Model::Draw(Material* material)
{
	UpdateModelMat();

    for (unsigned int i = 0; i < meshes.size(); i++) 
	{
        meshes[i].Draw(material);
    }
}

void Model::Bind(Material * material)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		Render::AddToCurrentDrawableList(&meshes[i], material, &modelMat);
	}
}

void Model::ChangeModelMat(glm::mat4 & mat)
{
	modelMat = mat;
}

void Model::UpdateModelMatsInList()
{
	for (auto i = modelList.begin(); i != modelList.end(); i++)
	{
		auto map = *i;
		Model* model = map.second;
		if (model && model->isDynamic)
			model->UpdateModelMat();
	}
}


// update model mat
inline void Model::UpdateModelMat()
{
	if (isDynamic)
	{
		modelMat = glm::mat4(1.0f);
		modelMat = glm::translate(modelMat, pos);
		modelMat = glm::rotate(modelMat, rotation.x, glm::vec3(1.0, 0.0, 0.0));
		modelMat = glm::rotate(modelMat, rotation.y, glm::vec3(0.0, 1.0, 0.0));
		modelMat = glm::rotate(modelMat, rotation.z, glm::vec3(0.0, 0.0, 1.0));
		modelMat = glm::scale(modelMat, scale);
	}
}