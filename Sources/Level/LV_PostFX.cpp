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
	s_mandelbrot = make_unique<Shader>("/Shaders/ScreenFX/ScreenFX.vs", "/Shaders/ScreenFX/Mandelbrot.fs");

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

void LV_PostFX::MandelbrotScene()
{
	// area x,y postition; area z scale; area w rotate angle
	static glm::vec4 area = glm::vec4(0.0, 0.0, 1.0, 1.0);
	static glm::vec4 smoothArea = glm::vec4(0.0, 0.0, 1.0, 1.0);

	// smooth control
	smoothArea = glm::mix(smoothArea, area, 0.03);

	if (Input::GetKeyOnce(GLFW_KEY_ENTER))
		s_mandelbrot->Reload();

	s_mandelbrot->use();

	area.x = std::clamp(area.x, -2.f, 2.f);
	area.y = std::clamp(area.y, -2.f, 2.f);
	area.z = area.z < 0.5 ? 0.5 : area.z;
	float moveSpeed = 0.005 / area.z;
	float scaleSpeed = 0.01;
	float rotSpeed = 0.01;
	float mandelbrotPrecise = 400.0f + area.z;

	float s = sin(area.w);
	float c = cos(area.w);

	glm::vec2 dirX = moveSpeed * glm::vec2(c , s);
	glm::vec2 dirY = glm::vec2(-dirX.y, dirX.x);

	if (Input::GetKey(GLFW_KEY_RIGHT))
		area.xy += dirX;
	if (Input::GetKey(GLFW_KEY_LEFT))
		area.xy -= dirX;
	if (Input::GetKey(GLFW_KEY_UP))
		area.xy += dirY;
	if (Input::GetKey(GLFW_KEY_DOWN))
		area.xy -= dirY;
	if (Input::GetKey(GLFW_KEY_MINUS))
	{
		area.z *= (1.0 - scaleSpeed);
	}
	if (Input::GetKey(GLFW_KEY_EQUAL))
	{
		area.z *= (1.0 + scaleSpeed);
	}
	if (Input::GetKey(GLFW_KEY_RIGHT_BRACKET))
		area.w += rotSpeed;
	if (Input::GetKey(GLFW_KEY_LEFT_BRACKET))
		area.w -= rotSpeed;

	s_mandelbrot->SetParam("iPrecise", mandelbrotPrecise);
	s_mandelbrot->SetParam("iArea", smoothArea);

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
	case LV_PostFX::Mandelbrot:
		MandelbrotScene();
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
	if (ImGui::Button("Mandelbrot"))
		currentScene = Mandelbrot;
}

