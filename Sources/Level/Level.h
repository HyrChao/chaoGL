#pragma once

#ifndef  LEVEL_H
#define LEVEL_H

#include <glad/glad.h>
#include <Render/Color.h>
#include <Render/Model.h>
#include<Render/Render.h>

class  Level
{
public:
	 Level();
	~ Level();

	void SetSkyDome(const char* path) 
	{
		unsigned int textureID = AssetsManager::CubeTextureFromFile(path);
		this->skydomeTex.id = textureID;
		this->skydomeTex.type = TextureType::Equirectangular;
		this->skydomeTex.path = path;
		skydomePath = path;
	}

	void SetSunLight(Light* sunlight)
	{
		this->sunlight = sunlight;
	}


protected:

	virtual void Loop();

private:

protected:

	// Skydome texture
	Texture skydomeTex;

	// Skydome resource path
	string skydomePath = "/Assets/Texture/HDR/Mans_Outside_2k.hdr";

	// Sun light (Main directional light)
	Light* sunlight;

	Material* skydomMaterial;

	bool initialized = false;

private:

	static Shader* skydomeShader;
	static bool globalInitialized;
	

};

#endif // ! LEVEL_H

