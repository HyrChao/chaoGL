
#include <Level/Level.h>


Shader* Level::skydomeShader;
bool Level::globalInitialized;

Level::Level()
{

	if (!globalInitialized)
	{
		skydomeShader = new Shader("/Shaders/Common/HDR_Cube.vs", "/Shaders/Common/HDR_Cube.fs");
		SetSkyDome(skydomePath.c_str());
		skydomMaterial = new Material(skydomeShader);
		skydomMaterial->AddTexture(skydomeTex);
		globalInitialized = true;
	}


}

Level::~Level()
{

}

void Level::Loop()
{
	glm::mat4 modelMat = glm::mat4(1.0f);
	//glm::translate(modelMat, glm::vec3(0.0f));
	glm::vec3 scale = glm::vec3(100);
	modelMat = glm::scale(modelMat, scale);
	Render::DrawCube(skydomMaterial, modelMat);
}

