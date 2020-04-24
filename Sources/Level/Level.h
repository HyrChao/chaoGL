#pragma once

#ifndef  LEVEL_H
#define LEVEL_H

#include <glad/glad.h>
#include <Render/Color.h>
#include <Render/Model.h>
#include<Render/Render.h>
#include "imgui.h"

class  Level
{
public:
	 
	Level();
	Level(string skyHDRTexture_path);

	virtual ~Level();

	void SetSunLight(Light* sunlight);

	Light* GetSunLight();


	virtual void Loop();
	virtual void OnGui();


protected:

	virtual void Initialize();

	virtual void SetupDefaultLight();

	virtual void CaptureEnvironment();

private:


	void Preload();

	void GlobalInitialize();

	void LoadEquirectangularSkydomeTexture();


	void DrawSkydome();

	void InitSingleInstance();


public:

	static Level* currentLevel;
	string name;


protected:

	// Skydome texture
	//Texture equirectangularSkyTex = Texture("/Assets/Texture/HDR/Mans_Outside_2k.hdr", TextureType::Equirectangular, false);
	Texture equirectangularSkyTex = Texture("/Assets/Texture/HDR/Ridgecrest_Road_Ref.hdr", Texture::TextureType::Equirectangular, false);

	// Sun light (Main directional light)
	Light* sunlight = nullptr;

	Material* skydomMaterial = nullptr;

	Texture envCubemap;
	Texture irradianceCubemap;
	Texture prefilterEnvironmentMap;
	static Texture prefilterBRDFLUT;

	DrawableList drawlist;

private:

	bool initialized = false;
	bool drawSkydome = false;
	bool reloadSkydome = false;

	static Shader* skydomeShader;
	static bool globalInitialized;


};

#endif // !LEVEL_H

