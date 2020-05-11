#include "Level/LV_PostFX.h"
#include "Application/Application.h"
#include "Input/Input.h"


LV_PostFX::LV_PostFX()
{

}

void LV_PostFX::Initialize()
{
	Level::Initialize();

	// Shader assets
	s_starTravelling = make_unique<Shader>("/Shaders/ScreenFX/ScreenFX.vs", "/Shaders/ScreenFX/StarTravelling.fs");
	s_starLink = make_unique<Shader>("/Shaders/ScreenFX/ScreenFX.vs", "/Shaders/ScreenFX/StarLink.fs");
	s_test = make_unique<Shader>("/Shaders/ScreenFX/ScreenFX.vs", "/Shaders/ScreenFX/PostFXTest.fs");
	s_rayMatching_Doughnut = make_unique<Shader>("/Shaders/ScreenFX/ScreenFX.vs", "/Shaders/ScreenFX/RayMatching_Doughnut.fs");
	s_mandelbrot = make_unique<Shader>("/Shaders/ScreenFX/ScreenFX.vs", "/Shaders/ScreenFX/Mandelbrot.fs");
	s_voronoi = make_unique<Shader>("/Shaders/ScreenFX/ScreenFX.vs", "/Shaders/ScreenFX/Voronoi.fs");

	// Texture assets
	t_mandelbrot_lut = Texture::LoadTexture("/Assets/Texture/LUT/Mandelbrot_LUT.png");

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

void LV_PostFX::StarLinkScene()
{
	s_starLink->use();
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

	if (Input::GetKey(GLFW_KEY_D))
		area.xy += dirX;
	if (Input::GetKey(GLFW_KEY_A))
		area.xy -= dirX;
	if (Input::GetKey(GLFW_KEY_W))
		area.xy += dirY;
	if (Input::GetKey(GLFW_KEY_S))
		area.xy -= dirY;
	if (Input::GetKey(GLFW_KEY_MINUS))
	{
		area.z *= (1.0 - scaleSpeed);
	}
	if (Input::GetKey(GLFW_KEY_EQUAL))
	{
		area.z *= (1.0 + scaleSpeed);
	}
	if (Input::GetKey(GLFW_KEY_RIGHT_BRACKET) || Input::GetKey(GLFW_KEY_Q))
		area.w += rotSpeed;
	if (Input::GetKey(GLFW_KEY_LEFT_BRACKET) || Input::GetKey(GLFW_KEY_E))
		area.w -= rotSpeed;

	s_mandelbrot->SetParam("iPrecise", mandelbrotPrecise);
	s_mandelbrot->SetParam("iArea", smoothArea);
	s_mandelbrot->SetParam("iColor", f_mandelbrot_lutColor);
	s_mandelbrot->SetParam("iRepeat", f_mandelbrot_repeat);

	t_mandelbrot_lut.Bind(s_mandelbrot.get(), "LUTMap", 0);

}

void LV_PostFX::VoronoiScene()
{
	s_voronoi->use();
}

void LV_PostFX::Loop()
{
	Level::Loop();


	// Reload Shaders
	if (Input::GetKeyOnce(GLFW_KEY_ENTER))
	{
		list<Shader*> shadersToReload;
		for (auto i = Shader::activeShaders.begin(); i != Shader::activeShaders.end(); i++)
		{
			Shader* shader = (*i).second;
			shadersToReload.push_back(shader);
		}
		for (auto i = shadersToReload.begin(); i != shadersToReload.end(); i++)
		{
			Shader* shader = (*i);
			shader->Reload();
		}
	}
	UpdatePostFXParameters();

	switch (currentScene)
	{
	case LV_PostFX::Test:
		TestScene();
		break;
	case LV_PostFX::StarTravelling:
		StarTravellingScene();
		break;
	case LV_PostFX::StarLink:
		StarLinkScene();
		break;
	case LV_PostFX::RayMarching:
		RayMatchingScene();
		break;
	case LV_PostFX::Mandelbrot:
		MandelbrotScene();
		break;
	case LV_PostFX::Voronoi:
		VoronoiScene();
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
	if (ImGui::Button("StarLink"))
		currentScene = StarLink;
	if (ImGui::Button("RayMatching"))
		currentScene = RayMarching;
	if (ImGui::Button("Mandelbrot"))
		currentScene = Mandelbrot;
	if (ImGui::Button("Voronoi"))
		currentScene = Voronoi;
	// Draw GUI on mandelbrot scene
	if (currentScene == Mandelbrot)
	{
		ImGui::SliderFloat("LUT Color", &f_mandelbrot_lutColor, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::SliderFloat("LUT_Repeat", &f_mandelbrot_repeat, 0.0f, 20.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	}
}

