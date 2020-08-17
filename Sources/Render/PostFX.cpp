#include "Render/PostFX.h"
#include "Render//Shader.h"
#include "Render/Texture.h"
#include "Application/Application.h"
#include "Input/Input.h"
#include <iostream>
#include <algorithm>

PostFX::PostFX()
{

}

void PostFX::OnGui(PostFXType postType)
{
	// Draw GUI on mandelbrot scene
	if (postType == Mandelbrot)
	{
		ImGui::SliderFloat("LUT Color", &f_mandelbrot_lutColor, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::SliderFloat("LUT_Repeat", &f_mandelbrot_repeat, 0.0f, 20.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	}
	// Draw GUI on KIFS scene
	if (postType == KIFS)
	{
		ImGui::SliderFloat("Segment Count", &f_KIFS_segments, 0.0f, 15.0f);
		ImGui::SliderFloat("Iterate Count", &f_KIFS_iterate, 0.0f, 8.0f);
	}
}

void PostFX::Initialize()
{


}

void PostFX::UpdatePostFXCommonParameters(Shader* currentPostShader)
{

	currentPostShader->use();
	currentPostShader->SetParam("iTime", Time::time);
	currentPostShader->SetParam("iResolution", Application::currentWindow->GetResolution());
	currentPostShader->SetParam("iMouse", Mouse::GetMouseParameter());
	currentPostShader->BindTexture("backBuffer", CommonAssets::instance->backBuffer);

}

void PostFX::Post_Test()
{
	if (Input::GetKeyOnce(GLFW_KEY_ENTER))
		s_test->Reload();
	if(s_test == nullptr)
		s_test = make_unique<Shader>("/Shaders/ScreenFX/ScreenFX.vs", "/Shaders/ScreenFX/PostFXTest.fs");

	UpdatePostFXCommonParameters(s_test.get());
	s_test->use();
}
void PostFX::Post_StarTravelling()
{
	if(s_starTravelling == nullptr)
		s_starTravelling = make_unique<Shader>("/Shaders/ScreenFX/ScreenFX.vs", "/Shaders/ScreenFX/StarTravelling.fs");

	UpdatePostFXCommonParameters(s_starTravelling.get());
	s_starTravelling->use();
}

void PostFX::Post_StarLink()
{
	if(s_starLink == nullptr)
		s_starLink = make_unique<Shader>("/Shaders/ScreenFX/ScreenFX.vs", "/Shaders/ScreenFX/StarLink.fs");

	UpdatePostFXCommonParameters(s_starLink.get());
	s_starLink->use();
}

void PostFX::Post_RayMatching()
{
	if(s_rayMatching_Doughnut == nullptr)
		s_rayMatching_Doughnut = make_unique<Shader>("/Shaders/ScreenFX/ScreenFX.vs", "/Shaders/ScreenFX/RayMatching_Doughnut.fs");

	UpdatePostFXCommonParameters(s_rayMatching_Doughnut.get());
	s_rayMatching_Doughnut->use();
}

void PostFX::Post_Mandelbrot()
{
	// area x,y postition; area z scale; area w rotate angle
	static glm::vec4 area = glm::vec4(0.0, 0.0, 1.0, 1.0);
	static glm::vec4 smoothArea = glm::vec4(0.0, 0.0, 1.0, 1.0);

	// smooth control
	smoothArea = glm::mix(smoothArea, area, 0.03);

	if(s_mandelbrot == nullptr)
		s_mandelbrot = make_unique<Shader>("/Shaders/ScreenFX/ScreenFX.vs", "/Shaders/ScreenFX/Mandelbrot.fs");

	// Texture assets
	if(t_mandelbrot_lut.id == 0)
		t_mandelbrot_lut = Texture::LoadTexture("/Assets/Texture/LUT/Mandelbrot_LUT.png");

	UpdatePostFXCommonParameters(s_mandelbrot.get());
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
	glm::vec2 dirX = moveSpeed * glm::vec2(c, s);
	glm::vec2 dirY = glm::vec2(-dirX.y, dirX.x);

	if (Input::GetKey(GLFW_KEY_D))
	{
		area.x += dirX.x;
		area.y += dirX.y;
	}
	if (Input::GetKey(GLFW_KEY_A))
	{
		area.x -= dirX.x;
		area.y -= dirX.y;
	}
	if (Input::GetKey(GLFW_KEY_W))
	{
		area.x += dirY.x;
		area.y += dirY.y;
	}
	if (Input::GetKey(GLFW_KEY_S))
	{
		area.x -= dirY.x;
		area.y -= dirY.y;
	}

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

	s_mandelbrot->BindTexture("LUTMap", t_mandelbrot_lut.id);

}

void PostFX::Post_Voronoi()
{
	if(s_voronoi == nullptr)
		s_voronoi = make_unique<Shader>("/Shaders/ScreenFX/ScreenFX.vs", "/Shaders/ScreenFX/Voronoi.fs");

	UpdatePostFXCommonParameters(s_voronoi.get());
	s_voronoi->use();
}

void PostFX::Post_KIFS()
{
	if(s_KIFS == nullptr)
		s_KIFS = make_unique<Shader>("/Shaders/ScreenFX/ScreenFX.vs", "/Shaders/ScreenFX/KIFS.fs");

	UpdatePostFXCommonParameters(s_KIFS.get());
	s_KIFS->use();
	s_KIFS->SetParam("Iterate", f_KIFS_iterate);
	s_KIFS->SetParam("Segments", f_KIFS_segments);
}

void PostFX::Post_Feather()
{
	if (s_feather == nullptr)
		s_feather = make_unique<Shader>("/Shaders/ScreenFX/ScreenFX.vs", "/Shaders/ScreenFX/FeatherFX.fs");

	UpdatePostFXCommonParameters(s_feather.get());
	s_feather->use();
}


bool PostFXManager::enablePostFX = true;
PostFX* PostFXManager::postFX = nullptr;
PostFX::PostFXType PostFXManager::postType = PostFX::PostFXType::Feather;


void PostFXManager::DrawPostFX()
{
	if (enablePostFX)
	{
		if (postFX == nullptr)
			postFX = new PostFX();

		postFX->Setup(postType);
	}
	else
	{
		CommonAssets::instance->drawGBufferShader->BindTexture("bufferTex", CommonAssets::instance->backBuffer);
	}
	RenderDevice::BindDefaultFrameBuffer();
	CommonAssets::DrawQuad();
}


void PostFXManager::PostFXGui()
{
	ImGui::Checkbox("Use PostFX", &enablePostFX);

	if (enablePostFX)
	{
		ImGui::Text("PostFX");

		if (ImGui::Button("Test"))
			postType = PostFX::PostFXType::Test;
		if (ImGui::Button("StarTravelling"))
			postType = PostFX::PostFXType::StarTravelling;
		if (ImGui::Button("StarLink"))
			postType = PostFX::PostFXType::StarLink;
		if (ImGui::Button("RayMatching"))
			postType = PostFX::PostFXType::RayMarching;
		if (ImGui::Button("Mandelbrot"))
			postType = PostFX::PostFXType::Mandelbrot;
		if (ImGui::Button("KIFS Fractals"))
			postType = PostFX::PostFXType::KIFS;
		if (ImGui::Button("Voronoi"))
			postType = PostFX::PostFXType::Voronoi;
		if (ImGui::Button("FeatherFX"))
			postType = PostFX::PostFXType::Feather;

		if (postFX == nullptr)
			postFX = new PostFX();

		postFX->OnGui(postType);
	}
}

void PostFX::Setup(PostFXType postFX)
{
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

	switch (postFX)
	{
	case Test:
		Post_Test();
		break;
	case StarTravelling:
		Post_StarTravelling();
		break;
	case StarLink:
		Post_StarLink();
		break;
	case RayMarching:
		Post_RayMatching();
		break;
	case Mandelbrot:
		Post_Mandelbrot();
		break;
	case KIFS:
		Post_KIFS();
		break;
	case Voronoi:
		Post_Voronoi();
		break;
	case Feather:
		Post_Feather();
		break;
	default:
		break;
	}
}
