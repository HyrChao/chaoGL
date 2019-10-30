#include <Section/Model_Section.h>



void Model_Section::Loop()
{
	PBR_Basic::Loop();

	glm::vec4 clearColor = glm::vec4(0.07f);
	clearColor.a = 1.0f;
	Render::SetClearColor(clearColor);
	if (!initialized) {

		Light::ClearAllLight();

		Light::LightParam pointLightp1;
		pointLightp1.type = Light::LightType::Point;
		pointLightp1.pos = glm::vec3(0.0f, 3.0f, 0.0f);
		pointLightp1.color = glm::vec3(0.6f, 0.2f, 0.2f);
		pointLightp1.constant = 1.0f;
		pointLightp1.linear = 0.09f;
		pointLightp1.quadratic = 0.032f;
		Light* pointLight1 = new Light(pointLightp1);

		Light::LightParam pointLightp2;
		pointLightp2.type = Light::LightType::Point;
		pointLightp2.pos = glm::vec3(-5.0f, -3.0f, -5.0f);
		pointLightp2.color = glm::vec3(0.1f, 0.8f, 0.1f);
		pointLightp2.constant = 1.0f;
		pointLightp2.linear = 0.7f;
		pointLightp2.quadratic = 1.8f;
		Light* pointLight2 = new Light(pointLightp2);

		Light::LightParam dirlightp1;
		dirlightp1.type = Light::LightType::Directional;
		dirlightp1.pos = glm::vec3(10.f, 10.f, 10.f);
		dirlightp1.color = glm::vec3(1.0f, 1.0f, 1.0f);
		dirlightp1.dir = glm::vec3(-1, -1, -1);;
		Light* dirLight = new Light(dirlightp1);

		Light::LightParam spotlightp1;
		spotlightp1.type = Light::LightType::Spot;
		spotlightp1.pos = glm::vec3(-3.0f, -1.0f, -8.0f);
		spotlightp1.color = glm::vec3(1.0f, 1.0f, 0.1f);
		spotlightp1.dir = glm::vec3(3.0f, 1.0f, 8.0f);
		spotlightp1.cutOffAngle = 30.0f;
		spotlightp1.outerCutoffAngle = 50.0f;
		Light* spotlight = new Light(spotlightp1);

		CommonAssets::instance->standardPBRShader->use();
		CommonAssets::instance->standardPBRShader->setInt("IBL.irradianceMap", TextureSlot::PBR_Irridiance);
		CommonAssets::instance->standardPBRShader->setInt("IBL.prefilterEnv", TextureSlot::PBR_Prefilter);
		CommonAssets::instance->standardPBRShader->setInt("IBL.BRDFPrefilterMap", TextureSlot::PBR_BRDF);

		if (model == nullptr)
		{
			model = new Model("/Assets/Model/pbr/shaderBall.fbx", false, glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.01));
		}


		initialized = true;
	}
	//float timeVal = glm::sin(Time::time) * 0.02f;
	float timeVal = Time::deltaTime;

	ChangeEnvironment();

	CommonAssets::instance->standardPBRShader->use();
	CommonAssets::instance->standardPBRShader->SetParam("debug_pbr", pbrDebugParam);
	CommonAssets::instance->standardPBRShader->SetParam("debug_light", lightDebugParam);

	glActiveTexture(GL_TEXTURE0 + TextureSlot::PBR_BRDF);
	glBindTexture(GL_TEXTURE_2D, prefilterBRDFLUT.id);
	glActiveTexture(GL_TEXTURE0 + TextureSlot::PBR_Irridiance);
	glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceCubemap.id);
	glActiveTexture(GL_TEXTURE0 + TextureSlot::PBR_Prefilter);
	glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterEnvironmentMap.id);

	model->Rotate(glm::vec3(0, timeVal, 0));
	model->Draw();

}