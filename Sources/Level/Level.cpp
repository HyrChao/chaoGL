
#include <Level/Level.h>


Shader* Level::skydomeShader;
bool Level::globalInitialized;

Level::Level()
{

	if (!globalInitialized)
	{
		equirectangularToCubemapMaterial = new Material("/Shaders/Common/HDR_EquirectangularMap.vs", "/Shaders/Common/HDR_EquirectangularMap.fs");
		SetSkyDome();
		//equirectangularToCubemapMaterial = new Material(equirectangularToCubemapShader);
		//skydomMaterial->AddTexture(skydomeTex);
		globalInitialized = true;
	}


}

Level::~Level()
{

}

void Level::Loop()
{
	DrawSkydome();
}

