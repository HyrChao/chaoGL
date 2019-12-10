#include <Section/Model_Section.h>


void Model_Section::LoadLevelResource()
{


}

void Model_Section::Initialize()
{
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
	InitPBR();
}

void Model_Section::Loop()
{
	PBR_Basic::Loop();

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



	CommonAssets::instance->standardPBRShader->use();
	CommonAssets::instance->standardPBRShader->SetParam("debug_pbr", pbrDebugParam);
	CommonAssets::instance->standardPBRShader->SetParam("debug_light", lightDebugParam);

	if (currentModelScene == ShadowScene)
	{
		// light params change with time
		float sunDirChangeSpeed = 0.3;
		glm::vec3 currentSunDir = glm::vec3(cos((float)glfwGetTime() * sunDirChangeSpeed), 0.5f + 0.5f * sin((float)glfwGetTime() * sunDirChangeSpeed), sin((float)glfwGetTime() * sunDirChangeSpeed));
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

		RockScene();
	}

}

void Model_Section::Shadowmapping()
{
	float scaleVal = 0.01f;
	glm::vec3 pos = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(scaleVal);
	glm::mat4 modelMat = glm::mat4(1.0f);

	if (!shadowSceneInitialized)
	{

		glm::vec3 pos = glm::vec3(0.0f);
		glm::vec3 rotation = glm::vec3(0.0f);
		glm::vec3 scale = glm::vec3(0.01f);

		if (groudModel == nullptr)
			groudModel = new Model("/Assets/Model/common/flatplane.fbx", false, pos, rotation, scale);

		if (mat_ground == nullptr)
			if (modelMat_grass == nullptr)
			{
				modelMat_grass = new Material(CommonAssets::instance->standardPBRShader, "/Assets/Model/pbr/tex_grass");
				mat_ground = modelMat_grass;
			}
			else
				mat_ground = modelMat_grass;

		if (colaModel == nullptr)
			colaModel = new Model("/Assets/Model/pbr/cola_can/cola_can.fbx", false, pos, rotation, scale);
		//colaModel = new Model("/Assets/Model/pbr/rock/sharprockfree.obj", false, pos, rotation, scale);

		if (mat_cola == nullptr)
			mat_cola = new Material(CommonAssets::instance->standardPBRShader, "/Assets/Model/pbr/cola_can");


		shadowSceneInitialized = true;
	}

	CommonAssets::instance->standardPBRShader->use();
	CommonAssets::instance->standardPBRShader->setMat4f("lightspaceMatrix", Shadow::lightspaceMat);
	CommonAssets::instance->standardPBRShader->setFloat("shadow.useShadow", 1.0f);
	CommonAssets::instance->standardPBRShader->setInt("shadow.shadowmap", TextureSlot::Shadowmap);
	glActiveTexture(GL_TEXTURE0 + TextureSlot::Shadowmap);
	glBindTexture(GL_TEXTURE_2D, Shadow::shadowMap.id);

	float pi = glm::pi<float>();
	modelMat = glm::rotate(modelMat,pi/2, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMat = glm::scale(modelMat, scale);
	modelMat = glm::translate(modelMat, glm::vec3(0.0, 0.0f, 10));
	groudModel->AddToDrawlist(mat_ground, modelMat);

	modelMat = glm::translate(modelMat, glm::vec3(0.0, 0.0f, -5));
	modelMat = glm::rotate(modelMat, -pi/2, glm::vec3(1.0, 0.0, 0.0));
	modelMat = glm::scale(modelMat, glm::vec3(30.0f));
	colaModel->AddToDrawlist(mat_cola, modelMat);
}

void Model_Section::ShaderBallScene() 
{
	drawlist.clear();

	float scaleVal = 0.01;
	glm::vec3 pos = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(scaleVal);
	glm::mat4 modelMat;

	if (!shaderballSceneInitialized)
	{
		if (model == nullptr)
		{
			//model = new Model("/Assets/Model/pbr/shaderBall.fbx", false, glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.01));

			model = new Model("/Assets/Model/pbr/shaderBall.fbx", false, pos, rotation, scale);
		}

		if (modelMat_gold == nullptr)
		{
			modelMat_gold = new Material(CommonAssets::instance->standardPBRShader, "/Assets/Model/pbr/tex_gold");
		}

		if (modelMat_grass == nullptr)
		{
			modelMat_grass = new Material(CommonAssets::instance->standardPBRShader, "/Assets/Model/pbr/tex_grass");
		}

		if (modelMat_wood == nullptr)
		{
			modelMat_wood = new Material(CommonAssets::instance->standardPBRShader, "/Assets/Model/pbr/tex_wood");
		}

		shaderballSceneInitialized = true;
	}




	modelMat = glm::rotate(modelMat, rotation.x, glm::vec3(1.0, 0, 0));
	modelMat = glm::rotate(modelMat, rotation.y, glm::vec3(0, 1.0, 0));
	modelMat = glm::rotate(modelMat, rotation.z, glm::vec3(0, 0, 1.0));
	modelMat = glm::scale(modelMat, scale);


	pos = glm::vec3(-3.0f, 0.0f, 0.0f);
	modelMat = glm::translate(modelMat, pos / scaleVal);
	model->Draw(modelMat_grass, modelMat);

	pos = glm::vec3(3.0f, 0.0f, 0.0f);
	modelMat = glm::translate(modelMat, pos / scaleVal);
	model->Draw(modelMat_gold, modelMat);

	modelMat = glm::translate(modelMat, pos / scaleVal);
	model->Draw(modelMat_wood, modelMat);

	//model->Rotate(glm::vec3(0, timeVal, 0));
}

void Model_Section::RockScene()
{
	drawlist.clear();

	float scaleVal = 0.01;
	glm::vec3 pos = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(scaleVal);
	glm::mat4 modelMat;

	if (!rockSceneInitialized)
	{
		if(rockModel == nullptr)
			rockModel = new Model("/Assets/Model/pbr/Temp/fireext/FireExt.fbx", false, pos, rotation, scale);

		if (rockMat == nullptr)
			rockMat = new Material(CommonAssets::instance->standardPBRShader, "/Assets/Model/pbr/Temp/fireext");

		rockSceneInitialized = true;
	}

	modelMat = glm::scale(modelMat, scale);

	rockModel->Draw(rockMat, modelMat);

}

