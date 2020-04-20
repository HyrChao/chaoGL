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

	float sunDirChangeSpeed;
	glm::vec3 currentSunDir;

	switch (currentModelScene)
	{
	case ShaderBalls:
		// light params change with time
		sunDirChangeSpeed = 0.3;
		currentSunDir = glm::vec3(cos((float)glfwGetTime() * sunDirChangeSpeed), sin((float)glfwGetTime() * sunDirChangeSpeed), 0.0f);
		sunlight->dir = currentSunDir;

		ShaderBallScene();
		break;

	case Rock:
		// light params change with time
		sunDirChangeSpeed = 0.3;
		currentSunDir = glm::vec3(cos((float)glfwGetTime() * sunDirChangeSpeed), sin((float)glfwGetTime() * sunDirChangeSpeed), 0.0f);
		sunlight->dir = currentSunDir;

		FireExtScene();
		break;

	default:
		break;
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
			m_model = make_unique<Model>("/Assets/Model/pbr/shaderBall.fbx", pos, rotation, scale, true, false);
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

	float rotateVal = Time::time * 0.2f;

	m_model->MoveTo(pos);
	m_model->SetRotation(glm::vec3(0.0f, rotateVal, 0.0f));
	m_model->SetScale(glm::vec3(0.5 * cos(Time::time * 0.2)) + 1.0f);
	m_model->Draw(m_modelMat_grass.get());

	pos = glm::vec3(-10.0f, 0.0f, 0.0f);
	m_model->MoveTo(pos);
	m_model->Draw(m_modelMat_gold.get());

	pos = glm::vec3(10.0f, 0.0f, 0.0f);
	m_model->MoveTo(pos);
	m_model->Draw(m_modelMat_wood.get());

}

void LV_Model::FireExtScene()
{
	drawlist.clear();

	float scaleVal = 0.01f;
	glm::vec3 pos = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(scaleVal);

	if (!rockSceneInitialized)
	{
		if (m_fireextModel == nullptr)
			m_fireextModel = make_unique<Model>("/Assets/Model/pbr/Temp/fireext/FireExt.fbx", pos, rotation, scale, false, false);

		if(m_rockMat == nullptr)
			m_rockMat = make_unique<Material>(CommonAssets::instance->standardPBRShader, "/Assets/Model/pbr/Temp/fireext");

		rockSceneInitialized = true;
	}

	m_fireextModel->SetScale(scale);
	m_fireextModel->Draw(m_rockMat.get());

}

