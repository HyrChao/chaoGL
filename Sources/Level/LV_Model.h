
#ifndef LV_Model_H
#define LV_Model_H

#include<Level/LV_Basic_PBR.h>
#include<memory>

class LV_Model : public LV_Basic_PBR
{
public:
	LV_Model() : LV_Basic_PBR("/Assets/Texture/HDR/Mans_Outside_2k.hdr")
	{

	}

	~LV_Model()
	{
	}


	void Loop() override;
	void OnGui() override;

protected:

	void Initialize() override;

private:

	void Shadowmapping();
	void ShaderBallScene();
	void FireExtScene();

private:

	bool shaderballSceneInitialized = false;
	bool rockSceneInitialized = false;
	bool shadowSceneInitialized = false;

	// Pointers that hold resource
	unique_ptr<Material> m_modelMat_gold = nullptr;
	unique_ptr<Material> m_modelMat_grass = nullptr;
	unique_ptr<Material> m_modelMat_wood = nullptr;

	unique_ptr<Material> m_rockMat = nullptr;
	unique_ptr<Material> m_mat_shaderball_spec = nullptr;
	unique_ptr<Material> m_mat_cola = nullptr;

	// Pointers that not hold resource
	Material* m_mat_ground = nullptr;

	unique_ptr<Model> m_model = nullptr;
	unique_ptr<Model> m_rockModel = nullptr;
	unique_ptr<Model> m_colaModel = nullptr;
	unique_ptr<Model> m_shaderballModel_spec = nullptr;
	unique_ptr<Model> m_groudModel = nullptr;
	enum Model_Scene
	{
		ShadowScene,
		ShaderBalls,
		Rock,
		LastScene
	};

	unsigned int currentModelScene = ShadowScene;

};

#endif // !LV_Model_H


