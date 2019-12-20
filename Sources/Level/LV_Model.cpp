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


	RegisterPBRShader(CommonAssets::instance->standardPBRShader);
	SetPBRShaderParams();
}


void LV_Model::Loop()
{
	LV_Basic_PBR::Loop();

	glm::vec4 clearColor = glm::vec4(0.07f);
	clearColor.a = 1.0f;
	Render::SetClearColor(clearColor);

	//float timeVal = glm::sin(Time::time) * 0.02f;
	float timeVal = Time::deltaTime;

	static bool shadowmapDebug;
	if (Input::GetKeyOnce(GLFW_KEY_CAPS_LOCK))
		if (shadowmapDebug)
			shadowmapDebug = false;
		else
			shadowmapDebug = true;

	if (shadowmapDebug)
		Render::DisplayFramebufferTexture(Shadow::shadowMap);
	//Render::DisplayFramebufferTexture(envCubemap);

	if (Input::GetKeyOnce(GLFW_KEY_TAB))
	{
		currentModelScene++;

		if (currentModelScene > (LastScene - 1))
			currentModelScene = 0;
	}

	if (currentModelScene == ShadowScene)
	{
		// light params change with time
		float sunDirChangeSpeed = 0.3;
		float scale = -1.0f;
		glm::vec3 currentSunDir = glm::vec3(cos((float)glfwGetTime() * sunDirChangeSpeed), -0.1f + scale + scale * sin((float)glfwGetTime() * sunDirChangeSpeed), sin((float)glfwGetTime() * sunDirChangeSpeed));
		sunlight->dir = currentSunDir;

		Shadowmapping();
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

void LV_Model::Shadowmapping()
{


	if (!shadowSceneInitialized)
	{

		glm::vec3 pos = glm::vec3(0.0f);
		glm::vec3 rotation = glm::vec3(0.0f);
		glm::vec3 scale = glm::vec3(1.0f);

		if (m_groudModel == nullptr)
			m_groudModel = make_unique<Model>("/Assets/Model/common/groundbox.fbx", false, pos, rotation, scale);

		if (m_mat_ground == nullptr)
			if (m_modelMat_wood == nullptr)
			{
				m_modelMat_wood = make_unique<Material>(CommonAssets::instance->standardPBRShader, "/Assets/Model/pbr/tex_wood");
				m_mat_ground = m_modelMat_wood.get();
			}
			else
				m_mat_ground = m_modelMat_wood.get();

		if (m_colaModel == nullptr)
			m_colaModel = make_unique<Model>("/Assets/Model/pbr/cola_can/cola_can.fbx", false, pos, rotation, scale);
		//colaModel = new Model("/Assets/Model/pbr/rock/sharprockfree.obj", false, pos, rotation, scale);

		if (m_shaderballModel_spec == nullptr)
			m_shaderballModel_spec = make_unique<Model>("/Assets/Model/pbr/shaderBall/shaderBall.fbx", false, pos, rotation, scale);

		if (m_mat_shaderball_spec == nullptr)
			m_mat_shaderball_spec = make_unique<Material>(CommonAssets::instance->standardPBRShader, "/Assets/Model/pbr/shaderBall");

		if (m_mat_cola == nullptr)
			m_mat_cola = make_unique<Material>(CommonAssets::instance->standardPBRShader, "/Assets/Model/pbr/cola_can");


		shadowSceneInitialized = true;
	}

	CommonAssets::instance->standardPBRShader->use();
	CommonAssets::instance->standardPBRShader->setMat4f("lightspaceMatrix", Shadow::lightspaceMat);
	CommonAssets::instance->standardPBRShader->setFloat("shadow.useShadow", 1.0f);
	CommonAssets::instance->standardPBRShader->setInt("shadow.shadowmap", TextureSlot::Shadowmap);
	glActiveTexture(GL_TEXTURE0 + TextureSlot::Shadowmap);
	glBindTexture(GL_TEXTURE_2D, Shadow::shadowMap.id);

	glm::mat4 model = glm::mat4(1.0f);
	float pi = glm::pi<float>();
	//modelMat = glm::rotate(modelMat,pi/2, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0, 0.0f, 0.0f));
	m_groudModel->AddToDrawlist(m_mat_ground, model);

	glm::mat4 model_cola = glm::mat4(1.0f);
	model_cola = glm::translate(model_cola, glm::vec3(-10.0, 0.0f, 0.0f));
	//modelMat = glm::rotate(modelMat, -pi/2, glm::vec3(1.0, 0.0, 0.0));
	//modelMat = glm::scale(modelMat, glm::vec3(1.0f));
	m_colaModel->AddToDrawlist(m_mat_cola.get(), model_cola);

	glm::mat4 model_sb = glm::mat4(1.0f);
	model_sb = glm::translate(model_sb, glm::vec3(10.0, 0.0f, 10.0f));
	m_shaderballModel_spec->AddToDrawlist(m_mat_shaderball_spec.get(), model_sb);
	//glm::mat4 model_cola2 = glm::mat4(1.0f);
	//model_cola = glm::translate(model_cola2, glm::vec3(-10.0, 30.0f, 0.0f));
	//colaModel->AddToDrawlist(mat_cola, model_cola2);
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

