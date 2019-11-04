//
//  Model.cpp
//  chaoGL
//
//  Created by Chao on 2019/4/16.
//

#include "Model.h"

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

	// process textures
	for (const auto & entry : std::experimental::filesystem::directory_iterator(directory))
	{
		// check all files in directory
		string texturePath = entry.path().string();
		string textureName = AssetsManager::ExtractFileName(texturePath);
		if (textureName.find(AssetsManager::TextureTypeToString(Texture::TextureType::Albedo)) != string::npos)
		{
			Texture textureLoaded = LoadTexture(texturePath, Texture::TextureType::Albedo);
			textures.push_back(textureLoaded);
			std::cout << "Load texture " << textureName <<" in path "<<texturePath << " to model " <<name<<std::endl;
		}
		
		if (textureName.find(AssetsManager::TextureTypeToString(Texture::TextureType::Normal)) != string::npos)
		{
			Texture textureLoaded = LoadTexture(texturePath, Texture::TextureType::Normal);
			textures.push_back(textureLoaded);
			std::cout << "Load texture " << textureName << " in path " << texturePath << " to model " << name << std::endl;
		}

		if (textureName.find(AssetsManager::TextureTypeToString(Texture::TextureType::Roughness)) != string::npos)
		{
			Texture textureLoaded = LoadTexture(texturePath, Texture::TextureType::Roughness);
			textures.push_back(textureLoaded);
			std::cout << "Load texture " << textureName << " in path " << texturePath << " to model " << name << std::endl;
		}

		if (textureName.find(AssetsManager::TextureTypeToString(Texture::TextureType::Metallic)) != string::npos)
		{
			Texture textureLoaded = LoadTexture(texturePath, Texture::TextureType::Metallic);
			textures.push_back(textureLoaded);
			std::cout << "Load texture " << textureName << " in path " << texturePath << " to model " << name << std::endl;
		}

		if (textureName.find(AssetsManager::TextureTypeToString(Texture::TextureType::MRO)) != string::npos)
		{
			Texture textureLoaded = LoadTexture(texturePath, Texture::TextureType::MRO);
			textures.push_back(textureLoaded);
			std::cout << "Load texture " << textureName << " in path " << texturePath << " to model " << name << std::endl;
		}

		if (textureName.find(AssetsManager::TextureTypeToString(Texture::TextureType::AO)) != string::npos)
		{
			Texture textureLoaded = LoadTexture(texturePath, Texture::TextureType::AO);
			textures.push_back(textureLoaded);
			std::cout << "Load texture " << textureName << " in path " << texturePath << " to model " << name << std::endl;
		}
	}

	LoadDefaultTexture(textures, Texture::Albedo);
	LoadDefaultTexture(textures, Texture::Normal);
	LoadDefaultTexture(textures, Texture::Roughness);
	LoadDefaultTexture(textures, Texture::Metallic);
	LoadDefaultTexture(textures, Texture::AO);

    return Mesh(vertices, indices, textures, this->modelMat);
}

void Model::ProcessTextures()
{

}

void Model::LoadDefaultTexture(vector<Texture>& textures, Texture::TextureType type)
{
	bool hasTex = false;
	for (auto i = textures.begin(); i != textures.end(); i++)
	{
		Texture tex = *i;
		if (tex.type == type)
		{
			hasTex = true;
			break;
		}
			
	}

	if (!hasTex)
	{
		Texture texture;
		texture.SetType(type);
		if (type == Texture::AO || type == Texture::Albedo)
			texture.id = CommonAssets::instance->whiteTex;
		else if (type == Texture::Normal)
			texture.id = CommonAssets::instance->flatNormal;
		else
			texture.id = CommonAssets::instance->blackTex;

		textures.push_back(texture);
	}
	
}

Texture Model::LoadTexture(string path, Texture::TextureType type)
{
	Texture texture;
	bool skip = false;
	for (unsigned int j = 0; j < AssetsManager::textures_loaded.size(); j++)
	{
		if (std::strcmp(AssetsManager::textures_loaded[j].path.data(), path.c_str()) == 0)
		{
			texture = AssetsManager::textures_loaded[j];
			skip = true;
			break;
		}
	}
	if (!skip)
	{
		texture.id = AssetsManager::TextureFromFile_FullPath(path.c_str());
		texture.SetType(type);
		texture.path = path.c_str();
		AssetsManager::textures_loaded.push_back(texture);
	}
	return texture;
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

void Model::Draw()
{
    
    for (unsigned int i = 0; i < meshes.size(); i++) 
	{     
        meshes[i].Draw();
    }
}
