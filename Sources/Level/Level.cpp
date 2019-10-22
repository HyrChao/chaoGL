
#include <Level/Level.h>


Shader* Level::skydomeShader;
bool Level::globalInitialized;


Level::~Level()
{

}

void Level::Loop()
{
	DrawSkydome();
}

