#pragma once
#include "Level.h"
#include <memory>



class LV_PostFX : public Level
{
public:

	LV_PostFX();

	void Loop() override;

	void OnGui() override;

protected:

	void Initialize() override;

	virtual void UpdatePostFXParameters();

private:

	void TestScene();

	void StarTravellingScene();

	void StarLinkScene();

	void RayMatchingScene();

	void MandelbrotScene();

	void VoronoiScene();


private:

	unique_ptr<Shader> s_starTravelling;
	unique_ptr<Shader> s_starLink;
	unique_ptr<Shader> s_rayMatching_Doughnut;
	unique_ptr<Shader> s_test;;
	unique_ptr<Shader> s_mandelbrot;
	unique_ptr<Shader> s_voronoi;


	Texture t_mandelbrot_lut;
	
	float f_mandelbrot_lutColor = 0.0;
	float f_mandelbrot_repeat = 1.0;

	enum PostFXScene 
	{
		Test,
		StarTravelling,
		StarLink,
		RayMarching,
		Mandelbrot,
		Voronoi
	};

	PostFXScene currentScene = PostFXScene::Mandelbrot;

};

