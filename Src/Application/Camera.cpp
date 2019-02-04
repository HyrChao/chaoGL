#include<Application/Camera.h>


int Camera::screenHeight;
int Camera::screenWidth;

Camera * Camera::main;

Camera::Camera()
{
	screenHeight = Application::screenHeight;
	screenWidth = Application::screenWidth;

	isPerspectiveMode = true;
	fov = 45.0f;; //field of view
	nearPLane = 0.1f;
	farPlane = 100.0f;

	SetView(isPerspectiveMode,fov,nearPLane,farPlane);
}

Camera::~Camera()
{
	delete this;
}

void Camera::SetView(bool isPerspective,float in_fov, float in_nearPlane, float in_farPlane)
{
	fov = in_fov;
	nearPLane = in_nearPlane;
	farPlane = in_farPlane;
	viewProjPers = glm::perspective(glm::radians(fov), (float)screenWidth / (float)screenHeight, nearPLane, farPlane);
	viewProjOrth = glm::ortho(0.0f, (float)screenWidth, 0.0f, (float)screenHeight, 0.1f, 100.0f);
	UseViewMode(isPerspective);
}

void Camera::UseViewMode(bool isPerspective)
{
	isPerspectiveMode = isPerspective;
	if (isPerspective)
	{
		projMat = viewProjPers;
	}
	else
	{
		projMat = viewProjOrth;
	}
}





