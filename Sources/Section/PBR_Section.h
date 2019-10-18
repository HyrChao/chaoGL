#ifndef PBR_SECTION_H
#define PBR_SECTION_H

#include <Level/Level.h>
#include<Input/Input.h>

class PBR_Section : public Level
{
private:

	Material* helloPBRMaterial = nullptr;
	Material* helloPBRMaterial_Fill = nullptr;
	Texture albedo, normal, metallic, roughness, ao;
	Texture albedo_Fill, normal_Fill, metallic_Fill, roughness_Fill, ao_Fill;


	Material* currentPBRMaterial;
	glm::vec4 pbrDebugParam = glm::vec4(0);
	glm::vec4 lightDebugParam = glm::vec4(0);

	Light* pbrSpotlight;
	Light* pbrPointlight1;

	glm::vec3 basicColor = glm::vec3(1.0f);


public:

	PBR_Section() : Level()
	{
		Level::Level();
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
