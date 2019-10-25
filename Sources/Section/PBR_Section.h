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

	float prefilterEnvMapRoughness_Max = 4.0f;
	float prefilterEnvMapRoughness = 0.0f;

	bool frameBufferDebug = false;

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

	void SwitchPBRScene(int sceneID)
	{
		switch (sceneID)
		{
		default:
			break;
		case RustMetalBalls:
			basicColor = glm::vec3(1.0f);
			currentPBRMaterial = helloPBRMaterial;
			BallsScene();
			break;
		case FlatColorBalls:
			basicColor = glm::vec3(0.5f);
			currentPBRMaterial = helloPBRMaterial_Fill;
			BallsScene();
			break;
		case MaterialBalls:
			break;

		}

	}

	// Scene
	void InitBallsScene();
	void BallsScene();
	void MaterialBallsScene();
	bool ballsSceneInitialized = false;
	// Debug
	void PrefilterEnvDebug();
	bool prefilterEnvDebugEnabled = false;
	float prefilterEnvDebugColdtime = 3.0;
	void FrameBufferDebug();
	void PBRMaterialDebug();

private:

	enum PBR_Scene
	{
		RustMetalBalls,
		FlatColorBalls,
		MaterialBalls,
		PBRModel,
		LastScene
	};

	unsigned int currentPBRScene = RustMetalBalls;




};


#endif // !PBR_SECTION_H
