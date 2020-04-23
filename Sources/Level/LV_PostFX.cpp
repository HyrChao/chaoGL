#include "Level/LV_PostFX.h"
#include "Application/Application.h"

LV_PostFX::LV_PostFX()
{

}

void LV_PostFX::Initialize()
{
	Level::Initialize();

	s_starTravelling = make_unique<Shader>("/Shaders/ScreenFX/ScreenFX.vs", "/Shaders/ScreenFX/StarTravelling.fs");

}

void LV_PostFX::UpdatePostFXParameters()
{
	for (auto i = Shader::activeShaders.begin(); i != Shader::activeShaders.end(); i++)
	{
		Shader* shader = (*i).second;
		shader->use();
		shader->SetParam("iTime", Time::time);
		shader->SetParam("iResolution", Application::currentWindow->GetResolution());
	}
}

void LV_PostFX::Loop()
{
	Level::Loop();

	UpdatePostFXParameters();

	s_starTravelling->use();
	CommonAssets::DrawQuad();
}

void LV_PostFX::OnGui()
{
	Level::OnGui();
}

