
#include <Level/Level.h>
#include "Level/LevelManager.h"

Shader* Level::skydomeShader;
bool Level::globalInitialized;
Texture Level::prefilterBRDFLUT;

Level::Level()
{
	Preload();
	Initialize();
	SetupDefaultLight();
}

Level::Level(string skyHDRTexture_path)
{
	equirectangularSkyTex.path = skyHDRTexture_path;
	Preload();
	Initialize();
	SetupDefaultLight();
}

Level::~Level()
{

}

void Level::Preload()
{
	LevelManager::SetCurrentLevel(this);

	GlobalInitialize();
	LoadEquirectangularSkydomeTexture();
	Capture::CaptureEnvironmentCubemap(equirectangularSkyTex, envCubemap);
	skydomMaterial = new Material(skydomeShader);
	skydomMaterial->AddTexture(envCubemap);

	SetSkyDome();
	DrawSkydome();

	drawlist.clear();
	Render::BindCurrentDrawableList(drawlist);
}

void Level::SetupDefaultLight()
{
	Light::ClearAllLight();

	Light::LightParam sunlightParam;
	sunlightParam.type = Light::LightType::Directional;
	sunlightParam.pos = glm::vec3(10.f, 10.f, 10.f);
	sunlightParam.color = glm::vec3(1.0f, 1.0f, 1.0f);
	sunlightParam.dir = glm::vec3(-1, -1, -1);;
	Light* dirLight = new Light(sunlightParam);

	sunlight = dirLight;
	Render::sunlight = sunlight;
}

void Level::Initialize()
{

}


void Level::Loop()
{
	DrawSkydome();
}

void Level::OnGui()
{
}

void Level::GlobalInitialize()
{
	if (!globalInitialized)
	{
		skydomeShader = new Shader("/Shaders/Common/Cube_Skydome.vs", "/Shaders/Common/Cube_Skydome.fs");
		globalInitialized = true;
	}
}

void Level::LoadEquirectangularSkydomeTexture()
{
	if (equirectangularSkyTex.id == 0)
	{
		unsigned int textureID = AssetsManager::CubeTextureFromFile(equirectangularSkyTex.path.c_str());
		equirectangularSkyTex.id = textureID;
		envCubemap.path = equirectangularSkyTex.path;
	}
}

void Level::SetSkyDome()
{

}

void Level::DrawSkydome()
{
	glm::mat4 modelMat = glm::mat4(1.0f);
	skydomMaterial->SetModelMat(modelMat);
	Render::SetMaterialCameraVP(skydomMaterial);
	skydomMaterial->use();
	CommonAssets::DrawCube();
}

void Level::SetSunLight(Light * sunlight)
{
	this->sunlight = sunlight;
}

Light* Level::GetSunLight()
{
	return this->sunlight;
}
