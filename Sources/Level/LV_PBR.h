#ifndef LV_PBR_H
#define LV_PBR_H

#include <Level/LV_Basic_PBR.h>
#include <memory>

class LV_PBR : public LV_Basic_PBR
{
public:

	LV_PBR() : LV_Basic_PBR("/Assets/Texture/HDR/Ridgecrest_Road_Ref.hdr")
	{

	}


	void Loop() override;

	void OnGui() override;

protected:

	void Initialize() override;

private:

	void SwitchPBRScene(int sceneID);

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

private:

	unique_ptr<Material> m_helloPBRMaterial = nullptr;
	unique_ptr<Material> m_helloPBRMaterial_Fill = nullptr;
	Material* m_currentPBRMaterial;

	Texture albedo, normal, metallic, roughness, ao;
	Texture albedo_Fill, normal_Fill, metallic_Fill, roughness_Fill, ao_Fill;

	Light* m_pbrSpotlight = nullptr;
	Light* m_pbrPointlight1 = nullptr;

	glm::vec3 basicColor = glm::vec3(1.0f);

	float prefilterEnvMapRoughness_Max = 4.0f;
	float prefilterEnvMapRoughness = 0.0f;

	bool frameBufferDebug = false;

	unsigned int currentPBRScene = RustMetalBalls;

	unique_ptr<Shader> m_helloPBRShader = nullptr;


private:

	enum PBR_Scene
	{
		RustMetalBalls,
		FlatColorBalls,
		MaterialBalls,
		PBRModel,
		LastScene
	};



};


#endif // !LV_PBR_H
