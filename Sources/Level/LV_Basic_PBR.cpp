
#include <Level/LV_Basic_PBR.h>

LV_Basic_PBR::LV_Basic_PBR() : Level()
{

}

LV_Basic_PBR::LV_Basic_PBR(string skyHDRPath) : Level(skyHDRPath)
{

}

LV_Basic_PBR::~LV_Basic_PBR()
{

}


void LV_Basic_PBR::Loop()
{
	Level::Loop();

	PBRMaterialDebug();
	BindPBRTextures();
}

void LV_Basic_PBR::OnGui()
{
	Level::OnGui();
}

void LV_Basic_PBR::Initialize()
{
	Capture::PrefilterBRDF(prefilterBRDFLUT);
	Capture::CaptureIrradianceCubemap(envCubemap, irradianceCubemap);
	Capture::CaptureSpecularPrefilterMap(envCubemap, prefilterEnvironmentMap);
	SetPBRShaderParams();
}

// Make sure use this funcction in RegisterPBRShaders()
void LV_Basic_PBR::RegisterPBRShader(Shader * shader)
{
	for (vector<Shader*>::iterator it = pbrShaders.begin(); it != pbrShaders.end(); it++)
	{
		if (*it == shader)
			return;
	}
	pbrShaders.push_back(shader);
}

void LV_Basic_PBR::DeregisterPBRShader(Shader * shader)
{
	for (vector<Shader*>::iterator it = pbrShaders.begin(); it != pbrShaders.end(); it++)
		if ((*it) == shader)
			pbrShaders.erase(it);
}

// Call after shaderregistered 
void LV_Basic_PBR::SetPBRShaderParams()
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
		shader->SetParam("debug_pbr", pbrDebugParam);
		shader->SetParam("debug_light", lightDebugParam);
		shader->SetParam("intensity.tint", basicColor);

	}
}

void LV_Basic_PBR::BindPBRTextures()
{
	glActiveTexture(GL_TEXTURE0 + TextureSlot::PBR_BRDF);
	glBindTexture(GL_TEXTURE_2D, prefilterBRDFLUT.id);
	glActiveTexture(GL_TEXTURE0 + TextureSlot::PBR_Irridiance);
	glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceCubemap.id);             
	glActiveTexture(GL_TEXTURE0 + TextureSlot::PBR_Prefilter);
	glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterEnvironmentMap.id);
}

void LV_Basic_PBR::DeregisterAllPBRShader()
{
	pbrShaders.clear();
}

void LV_Basic_PBR::PBRMaterialDebug()
{

	if (Input::GetKeyOnce(GLFW_KEY_TAB))
	{
		pbrDebugParam = glm::vec4(0);
		lightDebugParam = glm::vec4(0);
	}

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

