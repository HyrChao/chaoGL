
#include <Level/Level.h>

Shader* Level::skydomeShader;
bool Level::globalInitialized;
Texture Level::prefilterBRDFLUT;


Level::Level()
{
	GlobalInitialize();
	LoadEquirectangularSkydomeTexture();
	Capture::CaptureEnvironmentCubemap(equirectangularSkyTex, envCubemap);
	skydomMaterial = new Material(skydomeShader);
	skydomMaterial->AddTexture(envCubemap);
}

Level::Level(string skyHDRTexture_path)
{
	GlobalInitialize();
	equirectangularSkyTex.path = skyHDRTexture_path;
	LoadEquirectangularSkydomeTexture();
	Capture::CaptureEnvironmentCubemap(equirectangularSkyTex, envCubemap);
	skydomMaterial = new Material(skydomeShader);
	skydomMaterial->AddTexture(envCubemap);
}

Level::~Level()
{

}

void Level::Reset()
{
	initialized = false;
}

void Level::SetSunLight(Light * sunlight)
{
	this->sunlight = sunlight;
}

void Level::Loop()
{
	DrawSkydome();

	if (!resourceInitialized)
	{
		LoadLevelResource();

		SetSkyDome();

		resourceInitialized = true;
	}

	if (!initialized)
	{

		Initialize();

		drawlist.clear();
		Render::BindCurrentDrawableList(drawlist);

		Light::ClearAllLight();

		Light::LightParam sunlightParam;
		sunlightParam.type = Light::LightType::Directional;
		sunlightParam.pos = glm::vec3(10.f, 10.f, 10.f);
		sunlightParam.color = glm::vec3(1.0f, 1.0f, 1.0f);
		sunlightParam.dir = glm::vec3(-1, -1, -1);;
		Light* dirLight = new Light(sunlightParam);

		sunlight = dirLight;
		Render::sunlight = sunlight;

		initialized = true;
	}

}

void Level::ChangeEnvironment(Texture & envCubemap)
{
	skydomMaterial->ClearTextrues();
	skydomMaterial->AddTexture(envCubemap);
}


void Level::GlobalInitialize()
{
	if (!globalInitialized)
	{
		skydomeShader = new Shader("/Shaders/Common/Cube_Skydome.vs", "/Shaders/Common/Cube_Skydome.fs");
		Capture::PrefilterBRDF(prefilterBRDFLUT);
		globalInitialized = true;
	}
}

void Level::Initialize()
{

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

