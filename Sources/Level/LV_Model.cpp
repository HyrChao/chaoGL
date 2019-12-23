#include <Level/LV_Model.h>


void LV_Model::Initialize()
{
	LV_Basic_PBR::Initialize();

	if (sunlight)
	{
		Light::LightParam dirlightp1;
		dirlightp1.type = Light::LightType::Directional;
		dirlightp1.pos = glm::vec3(10.f, 10.f, 10.f);
		dirlightp1.color = glm::vec3(1.0f, 1.0f, 1.0f);
		dirlightp1.dir = glm::vec3(-1, -1, -1);;
		sunlight->SetLightParam(dirlightp1);
	}

}


void LV_Model::Loop()
{
	LV_Basic_PBR::Loop();

	glm::vec4 clearColor = glm::vec4(0.07f);
	clearColor.a = 1.0f;
	Render::SetClearColor(clearColor);

	//float timeVal = glm::sin(Time::time) * 0.02f;
	float timeVal = Time::deltaTime;

	if (Input::GetKeyOnce(GLFW_KEY_TAB))
	{
		currentModelScene++;

		if (currentModelScene > (LastScene - 1))
			currentModelScene = 0;
	}

	if (currentModelScene == ShaderBalls)
	{	
		// light params change with time
		float sunDirChangeSpeed = 0.3;
		glm::vec3 currentSunDir = glm::vec3(cos((float)glfwGetTime() * sunDirChangeSpeed), sin((float)glfwGetTime() * sunDirChangeSpeed), 0.0f);
		sunlight->dir = currentSunDir;

		ShaderBallScene();
	}

	if (currentModelScene == Rock)
	{
		// light params change with time
		float sunDirChangeSpeed = 0.3;
		glm::vec3 currentSunDir = glm::vec3(cos((float)glfwGetTime() * sunDirChangeSpeed), sin((float)glfwGetTime() * sunDirChangeSpeed), 0.0f);
		sunlight->dir = currentSunDir;

		FireExtScene();
	}

}

void LV_Model::OnGui()
{
	LV_Basic_PBR::OnGui();
}

void LV_Model::ShaderBallScene() 
{
	drawlist.clear();

	float scaleVal = 1.0f;
	glm::vec3 pos = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(scaleVal);
	glm::mat4 modelMat;

	if (!shaderballSceneInitialized)
	{
		if (m_model == nullptr)
		{
			m_model = make_unique<Model>("/Assets/Model/pbr/shaderBall.fbx", false, pos, rotation, scale);
		}

		if (m_modelMat_gold == nullptr)
		{
			m_modelMat_gold = make_unique<Material>(CommonAssets::instance->standardPBRShader, "/Assets/Model/pbr/tex_gold");
		}

		if (m_modelMat_grass == nullptr)
		{
			m_modelMat_grass = make_unique<Material>(CommonAssets::instance->standardPBRShader, "/Assets/Model/pbr/tex_grass");
		}

		if (m_modelMat_wood == nullptr)
		{
			m_modelMat_wood = make_unique<Material>(CommonAssets::instance->standardPBRShader, "/Assets/Model/pbr/tex_wood");
		}

		shaderballSceneInitialized = true;
	}




	modelMat = glm::rotate(modelMat, rotation.x, glm::vec3(1.0, 0, 0));
	modelMat = glm::rotate(modelMat, rotation.y, glm::vec3(0, 1.0, 0));
	modelMat = glm::rotate(modelMat, rotation.z, glm::vec3(0, 0, 1.0));
	modelMat = glm::scale(modelMat, scale);


	pos = glm::vec3(-10.0f, 0.0f, 0.0f);
	modelMat = glm::translate(modelMat, pos);
	m_model->Draw(m_modelMat_grass.get(), modelMat);

	pos = glm::vec3(10.0f, 0.0f, 0.0f);
	modelMat = glm::translate(modelMat, pos);
	m_model->Draw(m_modelMat_gold.get(), modelMat);

	pos = glm::vec3(10.0f, 0.0f, 0.0f);
	modelMat = glm::translate(modelMat, pos);
	m_model->Draw(m_modelMat_wood.get(), modelMat); 

	//model->Rotate(glm::vec3(0, timeVal, 0));
}

void LV_Model::FireExtScene()
{
	drawlist.clear();

	float scaleVal = 1.0f;
	glm::vec3 pos = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(scaleVal);
	glm::mat4 modelMat = glm::mat4(1.0f);

	if (!rockSceneInitialized)
	{
		if (m_rockModel == nullptr)
			m_rockModel = make_unique<Model>("/Assets/Model/pbr/Temp/fireext/FireExt.fbx", false, pos, rotation, scale);

		if(m_rockMat == nullptr)
			m_rockMat = make_unique<Material>(CommonAssets::instance->standardPBRShader, "/Assets/Model/pbr/Temp/fireext");

		rockSceneInitialized = true;
	}

	modelMat = glm::scale(modelMat, scale);

	m_rockModel->Draw(m_rockMat.get(), modelMat);

}

