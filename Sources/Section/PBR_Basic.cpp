
#include <Section/PBR_Basic.h>

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
}

