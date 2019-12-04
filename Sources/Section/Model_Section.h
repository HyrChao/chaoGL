
#ifndef MODEL_SECTION_H
#define MODEL_SECTION_H

#include<Section/PBR_Basic.h>

class Model_Section : public PBR_Basic
{
public:
	Model_Section() : PBR_Basic("/Assets/Texture/HDR/Mans_Outside_2k.hdr")
	{

	}

	~Model_Section()
	{
		delete model;
	}


	void Loop() override;


protected:

	void LoadLevelResource() override;
	void Initialize() override;

private:

	void Shadowmapping();
	void ShaderBallScene();
	void RockScene();

private:

	bool shaderballSceneInitialized = false;
	bool rockSceneInitialized = false;
	bool shadowSceneInitialized = false;

	Material* modelMat_gold = nullptr;
	Material* modelMat_grass = nullptr;
	Material* modelMat_wood = nullptr;
	Model* model = nullptr;

	Model* rockModel = nullptr;
	Material* rockMat = nullptr;

	Model* colaModel = nullptr;
	Material* mat_cola = nullptr;
	Model* groudModel = nullptr;
	Material* mat_ground = nullptr;

	enum Model_Scene
	{
		ShadowScene,
		ShaderBalls,
		Rock,
		LastScene
	};

	unsigned int currentModelScene = ShadowScene;

};

#endif // !MODEL_SECTION_H


