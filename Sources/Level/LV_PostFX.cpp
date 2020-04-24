#include "Level/LV_PostFX.h"
#include "Application/Application.h"
#include "Input/Input.h"
LV_PostFX::LV_PostFX()
{

}

void LV_PostFX::Initialize()
{
	Level::Initialize();

	s_starTravelling = make_unique<Shader>("/Shaders/ScreenFX/ScreenFX.vs", "/Shaders/ScreenFX/StarTravelling.fs");
	s_test = make_unique<Shader>("/Shaders/ScreenFX/ScreenFX.vs", "/Shaders/ScreenFX/PostFXTest.fs");
	s_rayMatching_Doughnut = make_unique<Shader>("/Shaders/ScreenFX/ScreenFX.vs", "/Shaders/ScreenFX/RayMatching_Doughnut.fs");

}

void LV_PostFX::UpdatePostFXParameters()
{
	for (auto i = Shader::activeShaders.begin(); i != Shader::activeShaders.end(); i++)
	{
		Shader* shader = (*i).second;
		shader->use();
		shader->SetParam("iTime", Time::time);
		shader->SetParam("iResolution", Application::currentWindow->GetResolution());
		shader->SetParam("iMouse", Mouse::GetMouseParameter());
	}
}

void LV_PostFX::TestScene()
{
	if (Input::GetKeyOnce(GLFW_KEY_ENTER))
		s_test->Reload();
	s_test->use();
}
void LV_PostFX::StarTravellingScene()
{
	s_starTravelling->use();
}

void LV_PostFX::RayMatchingScene()
{
	s_rayMatching_Doughnut->use();
}

void LV_PostFX::Loop()
{
	Level::Loop();

	UpdatePostFXParameters();


	switch (currentScene)
	{
	case LV_PostFX::Test:
		TestScene();
		break;
	case LV_PostFX::StarTravelling:
		StarTravellingScene();
		break;
	case LV_PostFX::RayMarching:
		RayMatchingScene();
		break;
	default:
		break;
	}

	CommonAssets::DrawQuad();
}

void LV_PostFX::OnGui()
{
	Level::OnGui();


	ImGui::Text("PostFX");

	if (ImGui::Button("Test"))
		currentScene = Test;
	if (ImGui::Button("StarTravelling"))
		currentScene = StarTravelling;
	if (ImGui::Button("RayMatching"))
		currentScene = RayMarching;
}

