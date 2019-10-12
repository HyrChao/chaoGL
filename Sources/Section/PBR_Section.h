#ifndef PBR_SECTION_H
#define PBR_SECTION_H

#include <Level/Level.h>
#include<Input/Input.h>

class PBR_Section : public Level
{
private:

	Shader* helloPBRShader;
	unsigned int albedo, normal, metallic, roughness, ao;
	Shader* helloPBRShader_Fill;

	Shader* currentPBRShader;
	glm::vec4 pbrDebugParam = glm::vec4(0);

	Light* pbrSpotlight;
	Light* pbrPointlight1;

public:

	PBR_Section() : Level()
	{
		Initialize();
	}

	void Reset()
	{
		initialized = false;
	}

	void Loop();

private:

	void Initialize();


};


#endif // !PBR_SECTION_H
