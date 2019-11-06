#include <Section/Model_Section.h>



void Model_Section::Loop()
{
	PBR_Basic::Loop();

	glm::vec4 clearColor = glm::vec4(0.07f);
	clearColor.a = 1.0f;
	Render::SetClearColor(clearColor);

	float scaleVal = 0.01;
	glm::vec3 pos = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(scaleVal);
	glm::mat4 modelMat;

	if (!initialized) {

		Light::ClearAllLight();

		//Light::LightParam pointLightp1;
		//pointLightp1.type = Light::LightType::Point;
		//pointLightp1.pos = glm::vec3(0.0f, 3.0f, 0.0f);
		//pointLightp1.color = glm::vec3(0.6f, 0.2f, 0.2f);
		//pointLightp1.constant = 1.0f;
		//pointLightp1.linear = 0.09f;
		//pointLightp1.quadratic = 0.032f;
		//Light* pointLight1 = new Light(pointLightp1);

		//Light::LightParam pointLightp2;
		//pointLightp2.type = Light::LightType::Point;
		//pointLightp2.pos = glm::vec3(-5.0f, -3.0f, -5.0f);
		//pointLightp2.color = glm::vec3(0.1f, 0.8f, 0.1f);
		//pointLightp2.constant = 1.0f;
		//pointLightp2.linear = 0.7f;
		//pointLightp2.quadratic = 1.8f;
		//Light* pointLight2 = new Light(pointLightp2);

		Light::LightParam dirlightp1;
		dirlightp1.type = Light::LightType::Directional;
		dirlightp1.pos = glm::vec3(10.f, 10.f, 10.f);
		dirlightp1.color = glm::vec3(1.0f, 1.0f, 1.0f);
		dirlightp1.dir = glm::vec3(-1, -1, -1);;
		sunlight = new Light(dirlightp1);

		//Light::LightParam spotlightp1;
		//spotlightp1.type = Light::LightType::Spot;
		//spotlightp1.pos = glm::vec3(-3.0f, -1.0f, -8.0f);
		//spotlightp1.color = glm::vec3(1.0f, 1.0f, 0.1f);
		//spotlightp1.dir = glm::vec3(3.0f, 1.0f, 8.0f);
		//spotlightp1.cutOffAngle = 30.0f;
		//spotlightp1.outerCutoffAngle = 50.0f;
		//Light* spotlight = new Light(spotlightp1);


		RegisterPBRShader(CommonAssets::instance->standardPBRShader);
		InitPBR();

		if (model == nullptr)
		{
			//model = new Model("/Assets/Model/pbr/shaderBall.fbx", false, glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.01));

			model = new Model("/Assets/Model/pbr//shaderBall.fbx", false, pos, rotation, scale);
		}

		if (modelMat_gold == nullptr)
		{
			modelMat_gold = new Material(CommonAssets::instance->standardPBRShader, "/Assets/Model/pbr/Temp/tex_gold");
		}

		if (modelMat_grass == nullptr)
		{
			modelMat_grass = new Material(CommonAssets::instance->standardPBRShader, "/Assets/Model/pbr/Temp/tex_grass");
		}

		if (modelMat_wood == nullptr)
		{
			modelMat_wood = new Material(CommonAssets::instance->standardPBRShader, "/Assets/Model/pbr/Temp/tex_wood");
		}

		initialized = true;
	}
	//float timeVal = glm::sin(Time::time) * 0.02f;
	float timeVal = Time::deltaTime;

	// light params change with time
	float sunDirChangeSpeed = 0.3;
	glm::vec3 currentSunDir = glm::vec3(cos((float)glfwGetTime() * sunDirChangeSpeed), sin((float)glfwGetTime() * sunDirChangeSpeed), 0.0f);
	sunlight->dir = currentSunDir;

	CommonAssets::instance->standardPBRShader->use();
	CommonAssets::instance->standardPBRShader->SetParam("debug_pbr", pbrDebugParam);
	CommonAssets::instance->standardPBRShader->SetParam("debug_light", lightDebugParam);

	modelMat = glm::rotate(modelMat, rotation.x, glm::vec3(1.0, 0, 0));
	modelMat = glm::rotate(modelMat, rotation.y, glm::vec3(0, 1.0, 0));
	modelMat = glm::rotate(modelMat, rotation.z, glm::vec3(0, 0, 1.0));
	modelMat = glm::scale(modelMat, scale);


	pos = glm::vec3(-3.0f, 0.0f, 0.0f);
	modelMat = glm::translate(modelMat, pos/scaleVal);
	model->Draw(modelMat_grass, modelMat);

	pos = glm::vec3(3.0f, 0.0f, 0.0f);
	modelMat = glm::translate(modelMat, pos / scaleVal);
	model->Draw(modelMat_gold, modelMat);

	modelMat = glm::translate(modelMat, pos / scaleVal);
	model->Draw(modelMat_wood, modelMat);

	//model->Rotate(glm::vec3(0, timeVal, 0));

}

