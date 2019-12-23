#include "Level/LV_Shadow.h"

void LV_Shadow::Initialize()
{
	LV_Basic_PBR::Initialize();

	glm::vec3 pos = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

	if (m_groudModel == nullptr)
		m_groudModel = make_unique<Model>("/Assets/Model/common/groundbox.fbx", false, pos, rotation, scale);

	if (m_mat_ground == nullptr)
		m_mat_ground = make_unique<Material>(CommonAssets::instance->standardPBRShader, "/Assets/Model/pbr/tex_wood");

	if (m_colaModel == nullptr)
		m_colaModel = make_unique<Model>("/Assets/Model/pbr/cola_can/cola_can.fbx", false, pos, rotation, scale);
	//colaModel = new Model("/Assets/Model/pbr/rock/sharprockfree.obj", false, pos, rotation, scale);

	if (m_shaderballModel_spec == nullptr)
		m_shaderballModel_spec = make_unique<Model>("/Assets/Model/pbr/shaderBall/shaderBall.fbx", false, pos, rotation, scale);

	if (m_mat_shaderball_spec == nullptr)
		m_mat_shaderball_spec = make_unique<Material>(CommonAssets::instance->standardPBRShader, "/Assets/Model/pbr/shaderBall");

	if (m_mat_cola == nullptr)
		m_mat_cola = make_unique<Material>(CommonAssets::instance->standardPBRShader, "/Assets/Model/pbr/cola_can");

}

void LV_Shadow::Loop()
{

	LV_Basic_PBR::Loop();

	static bool shadowmapDebug;
	if (Input::GetKeyOnce(GLFW_KEY_CAPS_LOCK))
		if (shadowmapDebug)
			shadowmapDebug = false;
		else
			shadowmapDebug = true;

	if (shadowmapDebug)
		Render::DisplayFramebufferTexture(Shadow::shadowMap);
	//Render::DisplayFramebufferTexture(envCubemap);

	// light params change with time
	float sunDirChangeSpeed = 0.3;
	float scale = -1.0f;
	glm::vec3 currentSunDir = glm::vec3(cos((float)glfwGetTime() * sunDirChangeSpeed), -0.1f + scale + scale * sin((float)glfwGetTime() * sunDirChangeSpeed), sin((float)glfwGetTime() * sunDirChangeSpeed));
	sunlight->dir = currentSunDir;

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
	m_groudModel->AddToDrawlist(m_mat_ground.get(), model);

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

void LV_Shadow::OnGui()
{
	LV_Basic_PBR::OnGui();
}

