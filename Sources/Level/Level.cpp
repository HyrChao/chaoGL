
#include <Level/Level.h>
#include "Level/LevelManager.h"

Shader* Level::skydomeShader;
bool Level::globalInitialized;
Texture Level::prefilterBRDFLUT;
Level* Level::currentLevel = nullptr;

Level::Level() 
{
}

Level::Level(string skyHDRTexture_path)
{
	equirectangularSkyTex.path = skyHDRTexture_path;
	drawSkydome = true;
	reloadSkydome = true;
}

Level::~Level()
{
	Render::UnbindCurrentDrawableList();
	Light::ClearAllLights();
}

void Level::Initialize()
{
	// Clear active shader while change level
	Shader::activeShaders.clear();
}

void Level::Preload()
{
	LevelManager::SetCurrentLevel(this);
	GlobalInitialize();

	CaptureEnvironment();
	// draw once for later environment capture
	DrawSkydome();

	drawlist.clear();
	Render::BindCurrentDrawableList(drawlist);
}

void Level::CaptureEnvironment()
{
	LoadEquirectangularSkydomeTexture();
	Capture::CaptureEnvironmentCubemap(equirectangularSkyTex, envCubemap);
	skydomMaterial = new Material(skydomeShader);
	skydomMaterial->AddTexture(envCubemap);
}

void Level::Loop()
{
	Level* currentLevel = LevelManager::GetCurrentLevel();
	if (currentLevel != this)
		initialized = false;

	if (!initialized)
	{
		Preload();
		Initialize();
		SetupDefaultLight();
		initialized = true;
	}

	if(drawSkydome)
		DrawSkydome();
	
	// Change current drawable list to this level's list on every loop begin
	Render::BindCurrentDrawableList(drawlist);

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


void Level::DrawSkydome()
{
	// sky dome depth is very far away from camera, disable depth test in case comfict 
	glDisable(GL_DEPTH_TEST);
	glm::mat4 modelMat = glm::mat4(1.0f);
	skydomMaterial->SetModelMat(modelMat);
	Render::SetMaterialCameraVP(skydomMaterial);
	skydomMaterial->use();
	CommonAssets::DrawCube();
	glEnable(GL_DEPTH_TEST);
}

void Level::SetSunLight(Light * sunlight)
{
	this->sunlight = sunlight;
}

Light* Level::GetSunLight()
{
	return this->sunlight;
}

void Level::SetupDefaultLight()
{
	Light::ClearAllLights();

	Light::LightParam sunlightParam;
	sunlightParam.type = Light::LightType::Directional;
	sunlightParam.pos = glm::vec3(10.f, 10.f, 10.f);
	sunlightParam.color = glm::vec3(1.0f, 1.0f, 1.0f);
	sunlightParam.dir = glm::vec3(-1, -1, -1);;
	Light* dirLight = new Light(sunlightParam);

	sunlight = dirLight;
	Render::sunlight = sunlight;
}
