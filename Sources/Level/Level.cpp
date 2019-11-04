
#include <Level/Level.h>

Shader* Level::skydomeShader;
bool Level::globalInitialized;
Texture Level::prefilterBRDFLUT;


Level::~Level()
{

}

void Level::Loop()
{
	DrawSkydome();
}

