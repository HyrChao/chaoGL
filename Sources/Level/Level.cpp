
#include <Level/Level.h>

Material* Level::skydomMaterial;
Shader* Level::skydomeShader;
bool Level::globalInitialized;


Level::~Level()
{

}

void Level::Loop()
{
	DrawSkydome();
}

