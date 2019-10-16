
#include <Level/Level.h>


Shader* Level::skydomeShader;
bool Level::globalInitialized;

Level::Level()
{

	if (!globalInitialized)
	{
		equirectangularToCubemapMaterial = new Material("/Shaders/Common/HDR_EquirectangularMap.vs", "/Shaders/Common/HDR_EquirectangularMap.fs");
		irradianceConvolveMaterial = new Material("/Shaders/Common/HDR_Cube_Irradiance.vs", "/Shaders/Common/HDR_Cube_Irradiance.fs");
		SetSkyDome();
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

