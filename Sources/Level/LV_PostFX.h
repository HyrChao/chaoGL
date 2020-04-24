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


private:

	unique_ptr<Shader> s_starTravelling;
	unique_ptr<Shader> s_rayMatching_Doughnut;
	unique_ptr<Shader> s_test;;

	enum PostFXScene 
	{
		Test,
		StarTravelling,
		RayMarching
	};

	PostFXScene currentScene = PostFXScene::Test;

};

