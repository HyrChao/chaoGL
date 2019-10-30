#include <Section/PBR_Section.h>



void PBR_Section::Initialize()
{
	Light::ClearAllLight();

	Light::LightParam pointLightp1;
	pointLightp1.type = Light::LightType::Point;
	pointLightp1.pos = glm::vec3(1.0f, 3.0f, 1.0f);
	pointLightp1.color = glm::vec3(1.0f, 0.02f, 0.1f);
	pointLightp1.constant = 1.0f;
	pointLightp1.linear = 0.09f;
	pointLightp1.quadratic = 0.032f;
	Light* pointLight1 = new Light(pointLightp1);
	pbrPointlight1 = pointLight1;

	Light::LightParam pointLightp2;
	pointLightp2.type = Light::LightType::Point;
	pointLightp2.pos = glm::vec3(-5.0f, -3.0f, -5.0f);
	pointLightp2.color = glm::vec3(0.1f, 0.8f, 0.1f);
	pointLightp2.constant = 1.0f;
	pointLightp2.linear = 0.7f;
	pointLightp2.quadratic = 1.8f;
	Light* pointLight2 = new Light(pointLightp2);

	Light::LightParam pointLightp3;
	pointLightp3.type = Light::LightType::Point;
	pointLightp3.pos = glm::vec3(3.0f, -4.0f, -5.0f);
	pointLightp3.color = glm::vec3(0.02f, 0.1f, 1.0f);
	pointLightp3.constant = 1.0f;
	pointLightp3.linear = 0.7f;
	pointLightp3.quadratic = 1.8f;
	Light* pointLight3 = new Light(pointLightp3);

	Light::LightParam dirlightp1;
	dirlightp1.type = Light::LightType::Directional;
	dirlightp1.pos = glm::vec3(10.f, 10.f, 10.f);
	dirlightp1.color = glm::vec3(1.0f, 1.0f, 1.0f);
	dirlightp1.dir = glm::vec3(-1, -1, -1);;
	Light* dirLight = new Light(dirlightp1);
	sunlight = dirLight;

	Light::LightParam spotlightp1;
	spotlightp1.type = Light::LightType::Spot;
	spotlightp1.pos = glm::vec3(-3.0f, -1.0f, -5.0f);
	spotlightp1.color = glm::vec3(1.0f, 1.0f, 0.1f);
	spotlightp1.dir = glm::vec3(1.0f, 0.0f, 0.0f);
	//spotlightp1.cutOffAngle = 12.5f;
	//spotlightp1.outerCutoffAngle = 17.5f;
	spotlightp1.cutOffAngle = 32.5f;
	spotlightp1.outerCutoffAngle = 47.5f;
	Light* spotlight = new Light(spotlightp1);
	pbrSpotlight = spotlight;

	initialized = true;
}

void PBR_Section::InitBallsScene()
{
	albedo.id = AssetsManager::TextureFromFile("/Assets/Texture/HelloPBR/rustediron2_basecolor.png");
	albedo.SetType(TextureType::Albedo);
	normal.id = AssetsManager::TextureFromFile("/Assets/Texture/HelloPBR/rustediron2_normal.png");
	normal.SetType(TextureType::Normal);
	metallic.id = AssetsManager::TextureFromFile("/Assets/Texture/HelloPBR/rustediron2_metallic.png");
	metallic.SetType(TextureType::Metallic);
	roughness.id = AssetsManager::TextureFromFile("/Assets/Texture/HelloPBR/rustediron2_roughness.png");
	roughness.SetType(TextureType::Roughness);
	//ao = AssetsManager::TextureFromFile(FileSystem::getPath("/Assets/Texture/white.png").c_str());
	ao.id = CommonAssets::instance->whiteTex;
	ao.SetType(TextureType::AO);

	albedo_Fill.id = CommonAssets::instance->whiteTex;
	albedo_Fill.SetType(TextureType::Albedo);
	normal_Fill.id = CommonAssets::instance->flatNormal;
	normal_Fill.SetType(TextureType::Normal);
	metallic_Fill.id = CommonAssets::instance->whiteTex;
	metallic_Fill.SetType(TextureType::Metallic);
	roughness_Fill.id = CommonAssets::instance->whiteTex;
	roughness_Fill.SetType(TextureType::Roughness);
	ao_Fill.id = CommonAssets::instance->whiteTex;
	ao_Fill.SetType(TextureType::AO);

	if (helloPBRMaterial == nullptr)
	{
		helloPBRMaterial = new Material(CommonAssets::instance->standardPBRShader);
		helloPBRMaterial->AddTexture(albedo);
		helloPBRMaterial->AddTexture(normal);
		helloPBRMaterial->AddTexture(metallic);
		helloPBRMaterial->AddTexture(roughness);
		helloPBRMaterial->AddTexture(ao);
		helloPBRMaterial->AddTexture(irradianceCubemap);
		helloPBRMaterial->AddTexture(prefilterEnvironmentMap);
		helloPBRMaterial->AddTexture(prefilterBRDFLUT);
	}

	if (helloPBRMaterial_Fill == nullptr)
	{
		helloPBRMaterial_Fill = new Material(CommonAssets::instance->standardPBRShader);
		helloPBRMaterial_Fill->AddTexture(albedo_Fill);
		helloPBRMaterial_Fill->AddTexture(normal_Fill);
		helloPBRMaterial_Fill->AddTexture(metallic_Fill);
		helloPBRMaterial_Fill->AddTexture(roughness_Fill);
		helloPBRMaterial_Fill->AddTexture(ao_Fill);
		helloPBRMaterial_Fill->AddTexture(irradianceCubemap);
		helloPBRMaterial_Fill->AddTexture(prefilterEnvironmentMap);
		helloPBRMaterial_Fill->AddTexture(prefilterBRDFLUT);
	}

	currentPBRMaterial = helloPBRMaterial;
	ballsSceneInitialized = true;
}

// Main loop
void PBR_Section::Loop()
{
	ChangeEnvironment();


	PrefilterEnvDebug();
	PBR_Basic::Loop();

	if (!initialized)
	{
		Initialize();
	}

	FrameBufferDebug();


	if (Input::GetKeyOnce(GLFW_KEY_TAB))
	{
		currentPBRScene++;

		if (currentPBRScene > (LastScene - 1))
			currentPBRScene = 0;
	}

	SwitchPBRScene(currentPBRScene);

}

void PBR_Section::BallsScene()
{
	if (!ballsSceneInitialized)
		InitBallsScene();

	glm::mat4 model = glm::mat4(1.0f);

	int nrRows = 7;
	int nrColumns = 7;
	float spacing = 2.5;

	glm::vec3 mroVar = glm::vec3(0.0f);

	// light params change with time
	float sunDirChangeSpeed = 0.3;
	glm::vec3 currentSunDir = glm::vec3(cos((float)glfwGetTime() * sunDirChangeSpeed), sin((float)glfwGetTime() * sunDirChangeSpeed), 0.0f);
	sunlight->dir = currentSunDir;

	float spotDirChangeSpeed = 1;
	glm::vec3 currentSpotDir = glm::vec3(cos((float)glfwGetTime() * spotDirChangeSpeed), 0.0f, sin((float)glfwGetTime() * spotDirChangeSpeed));
	pbrSpotlight->dir = currentSpotDir;

	float pointIntensityChangeSpeed = 2;
	float pointIntensityMutiplier = 1 + sin((float)glfwGetTime() * pointIntensityChangeSpeed);
	glm::vec3 currentPointColor = glm::vec3(pointIntensityMutiplier, 0.2 * pointIntensityMutiplier, 0.1 * pointIntensityMutiplier);
	pbrPointlight1->color = currentPointColor;

	mroVar.z = 1;

	currentPBRMaterial->BindTextures();
	currentPBRMaterial->SetParam("debug_pbr", pbrDebugParam);
	currentPBRMaterial->SetParam("debug_light", lightDebugParam);
	currentPBRMaterial->SetParam("intensity.tint", basicColor);

	for (int row = 0; row < nrRows; ++row)
	{
		for (int col = 0; col < nrColumns; ++col)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(
				(float)(col - (nrColumns / 2)) * spacing,
				(float)(row - (nrRows / 2)) * spacing,
				0.0f
			));

			mroVar.x = float(nrRows - row) / float(nrRows);
			mroVar.y = float(col + 1) / float(nrColumns);

			currentPBRMaterial->SetParam("intensity.mro", mroVar);
			currentPBRMaterial->SetModelMat(model);
			currentPBRMaterial->use();
			Render::DrawSphere();
		}
	}
}

void PBR_Section::MaterialBallsScene()
{

}

void PBR_Section::PrefilterEnvDebug()
{
	float maxColodtime = 3.0;
	float prefilterFadeSpeed = 0.5;
	if (Input::GetKey(GLFW_KEY_PAGE_DOWN))
	{
		prefilterEnvMapRoughness += Time::deltaTime * prefilterFadeSpeed;
		prefilterEnvDebugEnabled = true;
		prefilterEnvDebugColdtime = maxColodtime;
	}
	if (Input::GetKey(GLFW_KEY_PAGE_UP))
	{
		prefilterEnvMapRoughness -= Time::deltaTime * prefilterFadeSpeed;
		prefilterEnvDebugEnabled = true;
		prefilterEnvDebugColdtime = maxColodtime;
	}

	if (prefilterEnvDebugEnabled)
	{
		prefilterEnvDebugColdtime -= Time::deltaTime;
		if (prefilterEnvDebugColdtime < 0.0)
		{
			prefilterEnvDebugEnabled = false;
			prefilterEnvDebugColdtime = maxColodtime;
		}

		if (prefilterEnvMapRoughness > prefilterEnvMapRoughness_Max)
			prefilterEnvMapRoughness = prefilterEnvMapRoughness_Max;
		else
			if (prefilterEnvMapRoughness < 0)
			{
				prefilterEnvMapRoughness = 0.0f;
			}
		skydomMaterial->ReplaceTexture(envCubemap, prefilterEnvironmentMap);
		skydomMaterial->use();
		skydomMaterial->SetParam("mipLevel", prefilterEnvMapRoughness);
	}
	else 
	{
		skydomMaterial->ReplaceTexture(prefilterEnvironmentMap, envCubemap);
		skydomMaterial->use();
		skydomMaterial->SetParam("mipLevel", 0.0f);
		prefilterEnvMapRoughness = 0.0f;
	}
}

void PBR_Section::FrameBufferDebug()
{
	if (Input::GetKeyOnce(GLFW_KEY_CAPS_LOCK))
		if (frameBufferDebug)
			frameBufferDebug = false;
		else
			frameBufferDebug = true;

	if (frameBufferDebug)
		Render::DisplayFramebufferTexture(prefilterBRDFLUT);
		//Render::DisplayFramebufferTexture(envCubemap);
}
