#include <Section/PBR_Section.h>



void PBR_Section::Initialize()
{
	Light::ClearAllLight();

	LightParam pointLightp1;
	pointLightp1.type = LightType::Point;
	pointLightp1.pos = glm::vec3(1.0f, 3.0f, 1.0f);
	pointLightp1.color = glm::vec3(1.0f, 0.02f, 0.1f);
	pointLightp1.constant = 1.0f;
	pointLightp1.linear = 0.09f;
	pointLightp1.quadratic = 0.032f;
	Light* pointLight1 = new Light(pointLightp1);
	pbrPointlight1 = pointLight1;

	LightParam pointLightp2;
	pointLightp2.type = LightType::Point;
	pointLightp2.pos = glm::vec3(-5.0f, -3.0f, -5.0f);
	pointLightp2.color = glm::vec3(0.1f, 0.8f, 0.1f);
	pointLightp2.constant = 1.0f;
	pointLightp2.linear = 0.7f;
	pointLightp2.quadratic = 1.8f;
	Light* pointLight2 = new Light(pointLightp2);

	LightParam pointLightp3;
	pointLightp3.type = LightType::Point;
	pointLightp3.pos = glm::vec3(3.0f, -4.0f, -5.0f);
	pointLightp3.color = glm::vec3(0.02f, 0.1f, 1.0f);
	pointLightp3.constant = 1.0f;
	pointLightp3.linear = 0.7f;
	pointLightp3.quadratic = 1.8f;
	Light* pointLight3 = new Light(pointLightp3);

	LightParam dirlightp1;
	dirlightp1.type = LightType::Directional;
	dirlightp1.pos = glm::vec3(10.f, 10.f, 10.f);
	dirlightp1.color = glm::vec3(1.0f, 1.0f, 1.0f);
	dirlightp1.dir = glm::vec3(-1, -1, -1);;
	Light* dirLight = new Light(dirlightp1);
	sunlight = dirLight;

	LightParam spotlightp1;
	spotlightp1.type = LightType::Spot;
	spotlightp1.pos = glm::vec3(-3.0f, -1.0f, -5.0f);
	spotlightp1.color = glm::vec3(1.0f, 1.0f, 0.1f);
	spotlightp1.dir = glm::vec3(1.0f, 0.0f, 0.0f);
	//spotlightp1.cutOffAngle = 12.5f;
	//spotlightp1.outerCutoffAngle = 17.5f;
	spotlightp1.cutOffAngle = 32.5f;
	spotlightp1.outerCutoffAngle = 47.5f;
	Light* spotlight = new Light(spotlightp1);
	pbrSpotlight = spotlight;

	albedo = AssetsManager::TextureFromFile("/Assets/Texture/HelloPBR/rustediron2_basecolor.png");
	normal = AssetsManager::TextureFromFile("/Assets/Texture/HelloPBR/rustediron2_normal.png");
	metallic = AssetsManager::TextureFromFile("/Assets/Texture/HelloPBR/rustediron2_metallic.png");
	roughness = AssetsManager::TextureFromFile("/Assets/Texture/HelloPBR/rustediron2_roughness.png");
	//ao = AssetsManager::TextureFromFile(FileSystem::getPath("/Assets/Texture/white.png").c_str());
	ao = CommonAssets::instance->whiteTex;

	helloPBRShader = new Shader("/Shaders/Vertex/HelloPBR.vs", "/Shaders/Fragment/HelloPBR.fs");
	helloPBRShader_Fill = new Shader("/Shaders/Vertex/HelloPBR.vs", "/Shaders/Fragment/HelloPBR.fs");

	currentPBRShader = helloPBRShader;

	initialized = true;
}

void PBR_Section::Loop()
{
	Level::Loop();

	//glm::vec4 clearColor = glm::vec4(0.1f);
	//Render::SetClearColor(clearColor);

	if (!initialized)
	{
		Initialize();
	}

	helloPBRShader->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, albedo);
	glGenerateMipmap(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normal);
	glGenerateMipmap(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, metallic);
	glGenerateMipmap(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, roughness);
	glGenerateMipmap(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, ao);
	glGenerateMipmap(GL_TEXTURE_2D);

	helloPBRShader->setInt("material.albedo", 0);
	helloPBRShader->setInt("material.normal", 1);
	helloPBRShader->setInt("material.metallic", 2);
	helloPBRShader->setInt("material.roughness", 3);
	helloPBRShader->setInt("material.ao", 4);

	helloPBRShader->setVec3f("intensity.tint", glm::vec3(1.0f));
	helloPBRShader_Fill->setVec3f("intensity.mro", glm::vec3(1.0f));



	helloPBRShader_Fill->use();
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, CommonAssets::instance->whiteTex);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, CommonAssets::instance->whiteTex);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, CommonAssets::instance->whiteTex);
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, CommonAssets::instance->whiteTex);
	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, CommonAssets::instance->whiteTex);

	helloPBRShader_Fill->setInt("material.albedo", 5);
	helloPBRShader_Fill->setInt("material.normal", 6);
	helloPBRShader_Fill->setInt("material.metallic", 7);
	helloPBRShader_Fill->setInt("material.roughness", 8);
	helloPBRShader_Fill->setInt("material.ao", 9);

	helloPBRShader_Fill->setVec3f("intensity.tint", glm::vec3(1.0f, 0.0f, 0.0f));
	helloPBRShader_Fill->setVec3f("intensity.mro", glm::vec3(1.0f));

	glm::mat4 model = glm::mat4(1.0f);

	int nrRows = 7;
	int nrColumns = 7;
	float spacing = 2.5;

	glm::vec3 mroVar = glm::vec3(0.0f);

	if (Input::GetKeyOnce(GLFW_KEY_TAB))
	{
		pbrDebugParam = glm::vec4(0);
		if (currentPBRShader == helloPBRShader)
			currentPBRShader = helloPBRShader_Fill;
		else
			currentPBRShader = helloPBRShader;
	}

	if (Input::GetKeyOnce(GLFW_KEY_F1))
	{
		pbrDebugParam = glm::vec4(0);
		pbrDebugParam.x = 1;
	}
	else if (Input::GetKeyOnce(GLFW_KEY_F2))
	{
		pbrDebugParam = glm::vec4(0);
		pbrDebugParam.y = 1;
	}
	else if (Input::GetKeyOnce(GLFW_KEY_F3))
	{
		pbrDebugParam = glm::vec4(0);
		pbrDebugParam.z = 1;
	}
	else if (Input::GetKeyOnce(GLFW_KEY_F4))
	{
		pbrDebugParam = glm::vec4(0);
		pbrDebugParam.w = 1;
	}

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
			mroVar.y = float(col) / float(nrColumns);
			currentPBRShader->use();
			currentPBRShader->setVec4f("debug", pbrDebugParam);
			currentPBRShader->setVec3f("intensity.mro", mroVar);

			Render::DrawSphere(currentPBRShader, model);
		}
	}
}
