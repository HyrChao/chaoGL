#pragma once

#include <memory>
#include <Render/Shader.h>
#include <Render/Texture.h>
class PostFX 
{

public:

	enum PostFXType
	{
		Test,
		StarTravelling,
		StarLink,
		RayMarching,
		Mandelbrot,
		Voronoi,
		KIFS,
		Feather
	};

public:

	PostFX();

	void Setup(PostFXType postFX);

	void OnGui(PostFXType postType);

private:

	void Initialize();

	void UpdatePostFXCommonParameters(Shader* currentPostShader);

	void Post_Test();

	void Post_StarTravelling();

	void Post_StarLink();

	void Post_RayMatching();

	void Post_Mandelbrot();

	void Post_Voronoi();

	void Post_KIFS();

	void Post_Feather();

private:

	unique_ptr<Shader> s_starTravelling = nullptr;
	unique_ptr<Shader> s_starLink = nullptr;
	unique_ptr<Shader> s_rayMatching_Doughnut = nullptr;
	unique_ptr<Shader> s_test = nullptr;
	unique_ptr<Shader> s_mandelbrot = nullptr;
	unique_ptr<Shader> s_voronoi = nullptr;
	unique_ptr<Shader> s_KIFS = nullptr;
	unique_ptr<Shader> s_feather = nullptr;


	Texture t_mandelbrot_lut;

	float f_mandelbrot_lutColor = 0.0;
	float f_mandelbrot_repeat = 1.0;

	float f_KIFS_iterate = 1.0;
	float f_KIFS_segments = 3.0;

};

static class PostFXManager
{
public:
	static void DrawPostFX();
	static void PostFXGui();

private:
	static bool enablePostFX;
	static PostFX* postFX;
	static PostFX::PostFXType postType;
private:
	PostFXManager() = delete;
	~PostFXManager() = delete;
};

