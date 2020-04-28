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

	void RayMatchingScene();

	void MandelbrotScene();


private:

	unique_ptr<Shader> s_starTravelling;
	unique_ptr<Shader> s_rayMatching_Doughnut;
	unique_ptr<Shader> s_test;;
	unique_ptr<Shader> s_mandelbrot;


	Texture t_mandelbrot_lut;
	
	float f_mandelbrot_lutColor = 0.0;

	enum PostFXScene 
	{
		Test,
		StarTravelling,
		RayMarching,
		Mandelbrot
	};

	PostFXScene currentScene = PostFXScene::Mandelbrot;

};

