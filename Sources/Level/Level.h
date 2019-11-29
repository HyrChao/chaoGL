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
	Level(string skyHDRTexture_path);

	virtual ~Level();

	void Reset();

	void SetSunLight(Light* sunlight);


	virtual void Loop();

protected:

	virtual void ChangeEnvironment(Texture& envCubemap);

	virtual void LoadLevelResource() = 0;

	virtual void Initialize()= 0;

private:



	void GlobalInitialize();


	void LoadEquirectangularSkydomeTexture();

	void SetSkyDome();

	void DrawSkydome();




protected:

	// Skydome texture
	//Texture equirectangularSkyTex = Texture("/Assets/Texture/HDR/Mans_Outside_2k.hdr", TextureType::Equirectangular, false);
	Texture equirectangularSkyTex = Texture("/Assets/Texture/HDR/Ridgecrest_Road_Ref.hdr", Texture::TextureType::Equirectangular, false);

	// Sun light (Main directional light)
	Light* sunlight = nullptr;

	Material* skydomMaterial = nullptr;

	bool resourceLoaded = false;
	bool initialized = false;
	bool resourceInitialized = false;

	Texture envCubemap;
	Texture irradianceCubemap;
	Texture prefilterEnvironmentMap;
	static Texture prefilterBRDFLUT;

	DrawableList drawlist;

private:

	static Shader* skydomeShader;
	static bool globalInitialized;


};

#endif // !LEVEL_H

