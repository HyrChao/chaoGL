
#include <Section/PBR_Basic.h>

PBR_Basic::PBR_Basic() : Level()
{
	Capture::CaptureIrradianceCubemap(envCubemap, irradianceCubemap);
	Capture::CaptureSpecularPrefilterMap(envCubemap, prefilterEnvironmentMap);
}

PBR_Basic::PBR_Basic(string skyHDRPath) : Level(skyHDRPath)
{

	Capture::CaptureIrradianceCubemap(envCubemap, irradianceCubemap);
	Capture::CaptureSpecularPrefilterMap(envCubemap, prefilterEnvironmentMap);
}

PBR_Basic::~PBR_Basic()
{

}


void PBR_Basic::Loop()
{
	Level::Loop();

	if (Input::GetKeyOnce(GLFW_KEY_TAB))
	{
		pbrDebugParam = glm::vec4(0);
		lightDebugParam = glm::vec4(0);
	}

	PBRMaterialDebug();
	BindPBRTextures();
}

void PBR_Basic::Initialize()
{

}

void PBR_Basic::RegisterPBRShader(Shader * shader)
{
	for (vector<Shader*>::iterator it = pbrShaders.begin(); it != pbrShaders.end(); it++)
	{
		if (*it == shader)
			return;
	}
	pbrShaders.push_back(shader);
}

void PBR_Basic::DeregisterPBRShader(Shader * shader)
{
	for (vector<Shader*>::iterator it = pbrShaders.begin(); it != pbrShaders.end(); it++)
		if ((*it) == shader)
			pbrShaders.erase(it);
}

// Call after shader registered 

void PBR_Basic::InitPBR()
{
	for (auto it = pbrShaders.begin(); it != pbrShaders.end(); it++)
	{
		Shader* shader = *it;
		shader->use();
		shader->setInt("IBL.irradianceMap", TextureSlot::PBR_Irridiance);
		shader->setInt("IBL.prefilterEnv", TextureSlot::PBR_Prefilter);
		shader->setInt("IBL.BRDFPrefilterMap", TextureSlot::PBR_BRDF);
		glm::vec3 basicColor = glm::vec3(1.0f);
		glm::vec3 basicMRO = glm::vec3(1.0f);
		shader->setVec3f("intensity.tint", basicColor);
		shader->setVec3f("intensity.mro", basicMRO);
	}
}

void PBR_Basic::BindPBRTextures()
{
	glActiveTexture(GL_TEXTURE0 + TextureSlot::PBR_BRDF);
	glBindTexture(GL_TEXTURE_2D, prefilterBRDFLUT.id);
	glActiveTexture(GL_TEXTURE0 + TextureSlot::PBR_Irridiance);
	glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceCubemap.id);
	glActiveTexture(GL_TEXTURE0 + TextureSlot::PBR_Prefilter);
	glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterEnvironmentMap.id);
}

void PBR_Basic::DeregisterAllPBRShader()
{
	pbrShaders.clear();
}

void PBR_Basic::PBRMaterialDebug()
{
	if (Input::GetKeyOnce(GLFW_KEY_F1))
	{
		pbrDebugParam = glm::vec4(0);
		lightDebugParam = glm::vec4(0);
		pbrDebugParam.x = 1;
	}
	else if (Input::GetKeyOnce(GLFW_KEY_F2))
	{
		pbrDebugParam = glm::vec4(0);
		lightDebugParam = glm::vec4(0);
		pbrDebugParam.y = 1;
	}
	else if (Input::GetKeyOnce(GLFW_KEY_F3))
	{
		pbrDebugParam = glm::vec4(0);
		lightDebugParam = glm::vec4(0);
		pbrDebugParam.z = 1;
	}
	else if (Input::GetKeyOnce(GLFW_KEY_F4))
	{
		pbrDebugParam = glm::vec4(0);
		lightDebugParam = glm::vec4(0);
		pbrDebugParam.w = 1;
	}
	if (Input::GetKeyOnce(GLFW_KEY_F5))
	{
		pbrDebugParam = glm::vec4(0);
		lightDebugParam = glm::vec4(0);
		lightDebugParam.x = 1;
	}
	if (Input::GetKeyOnce(GLFW_KEY_F6))
	{
		pbrDebugParam = glm::vec4(0);
		lightDebugParam = glm::vec4(0);
		lightDebugParam.y = 1;
	}
	if (Input::GetKeyOnce(GLFW_KEY_F7))
	{
		pbrDebugParam = glm::vec4(0);
		lightDebugParam = glm::vec4(0);
		lightDebugParam.z = 1;
	}
}

