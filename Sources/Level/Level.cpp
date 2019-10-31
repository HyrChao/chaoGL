
#include <Level/Level.h>

Material* Level::skydomMaterial;
Shader* Level::skydomeShader;
bool Level::globalInitialized;

unsigned int Level::captureFBO;
unsigned int Level::captureRBO;

Level::~Level()
{

}

void Level::Loop()
{
	DrawSkydome();
}

