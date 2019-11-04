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
	 
	 Level()
	 {
		 GlobalInitialize();
		 LevelInitialize();
	 }
	 Level(string skyHDRTexture_path)
	 {
		GlobalInitialize();
		equirectangularSkyTex.path = skyHDRTexture_path;
		LevelInitialize();
	 }

	~ Level();

	void Reset()
	{
		initialized = false;
	}

	void SetSunLight(Light* sunlight)
	{
		this->sunlight = sunlight;
	}



protected:

	virtual void Loop();

	virtual void ChangeEnvironment(Texture& envCubemap)
	{
		skydomMaterial->ClearTextrues();
		skydomMaterial->AddTexture(envCubemap);
	}

private:



	void GlobalInitialize()
	{
		if (!globalInitialized)
		{
			skydomeShader = new Shader("/Shaders/Common/Cube_Skydome.vs", "/Shaders/Common/Cube_Skydome.fs");
			Capture::PrefilterBRDF(prefilterBRDFLUT);
			globalInitialized = true;
		}
	}

	void LevelInitialize()
	{
		LoadEquirectangularSkydomeTexture();
		Capture::CaptureEnvironmentCubemap(equirectangularSkyTex, envCubemap);
		Capture::CaptureIrradianceCubemap(envCubemap, irradianceCubemap);
		Capture::CaptureSpecularPrefilterMap(envCubemap, prefilterEnvironmentMap);
		skydomMaterial = new Material(skydomeShader);
		skydomMaterial->AddTexture(envCubemap);
	}

	void LoadEquirectangularSkydomeTexture()
	{
		if (equirectangularSkyTex.id == 0)
		{
			unsigned int textureID = AssetsManager::CubeTextureFromFile(equirectangularSkyTex.path.c_str());
			equirectangularSkyTex.id = textureID;
			envCubemap.path = equirectangularSkyTex.path;
		}
	}

	void SetSkyDome()
	{

	}

	void DrawSkydome()
	{
		glm::mat4 modelMat = glm::mat4(1.0f);
		skydomMaterial->SetModelMat(modelMat);
		Render::SetMaterialCameraVP(skydomMaterial);
		skydomMaterial->use();
		CommonAssets::DrawCube();
	}




protected:

	// Skydome texture
	//Texture equirectangularSkyTex = Texture("/Assets/Texture/HDR/Mans_Outside_2k.hdr", TextureType::Equirectangular, false);
	Texture equirectangularSkyTex = Texture("/Assets/Texture/HDR/Ridgecrest_Road_Ref.hdr", Texture::TextureType::Equirectangular, false);

	// Sun light (Main directional light)
	Light* sunlight;

	Material* skydomMaterial;

	bool resourceLoaded = false;
	bool initialized = false;

	Texture envCubemap;
	Texture irradianceCubemap;
	Texture prefilterEnvironmentMap;
	static Texture prefilterBRDFLUT;

private:

	static Shader* skydomeShader;
	static bool globalInitialized;


};

#endif // !LEVEL_H

