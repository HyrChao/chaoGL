#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Application/Application.h>

class Camera
{
public:
	Camera();
	~Camera();

	static glm::mat4 GetPerspectiveMat()
	{
		return main->viewProjPers;
	}

	static glm::mat4 GetOrthographicMat()
	{
		return main->viewProjOrth;
	}

	static glm::mat4 GetViewProjectMat()
	{
		return main->projMat;
	}

	void SetView(bool isPerspective,float in_fov, float in_nearPlane, float in_farPlane);

	void UseViewMode(bool isPerspective);

public:

	static Camera * main;
	glm::mat4 projMat;

private:

	glm::mat4 viewProjPers;
	glm::mat4 viewProjOrth;
	float fov;
	float nearPLane;
	float farPlane;
	bool isPerspectiveMode;

	static int screenWidth;
	static int screenHeight;

};
#endif
