#ifndef PBR_SECTION_H
#define PBR_SECTION_H

#include <Section/PBR_Section.h>

#include <glad/glad.h>
#include <Render/Render.h>
#include <Render/Color.h>
#include <Render/Model.h>

#include<Input/Input.h>

class PBR_Section
{
private:

	bool initialized = false;

	Shader* helloPBRShader;
	unsigned int albedo, normal, metallic, roughness, ao;
	Shader* helloPBRShader_Fill;

	Shader* currentPBRShader;
	glm::vec4 pbrDebugParam = glm::vec4(0);

	Light* pbrDirlight;
	Light* pbrSpotlight;
	Light* pbrPointlight1;



public:

	void Reset()
	{
		initialized = false;
	}

	void Loop();

private:

	void Initialize();

};


#endif // !PBR_SECTION_H
