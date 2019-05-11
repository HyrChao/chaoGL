#pragma once
#ifndef CAMERA_H
#define CAMERA_H
#include <Application/Time.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Camera
{
public:

	Camera(int in_screenWidth, int in_screenHeight);
	~Camera();

	void UpdateCamera();

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

	void SetView(bool isPerspective, float in_fov, float in_nearPlane, float in_farPlane);
	void SetScreenExtend(int in_screenWidth, int in_screenHeight);
	void PerspectiveMode(bool isPerspective = true);
	void Move(glm::vec3 pos);
	void MoveView(float yawoffset, float pitchoffset);
	void LockAt(glm::vec3 targetPos);
	void LockAtUnlock();
	void CameraAutoSpan();
	void MoveForward(float speed);
	void MoveRight(float speed);

private:


public:

	glm::vec3 pos;
	static Camera * main;
	glm::mat4 projMat;
	glm::mat4 viewMat;

private:

	bool lookAt = false;

	glm::vec3 cameraFront;
	glm::vec3 cameraTarget;
	glm::vec3 cameraDirection;
	glm::vec3 cameraRight;
	glm::vec3 cameraUp;
	glm::vec3 cameraLockonTarget;

	glm::mat4 viewProjPers;
	glm::mat4 viewProjOrth;

	float fov;
	float nearPLane;
	float farPlane;
	bool isPerspectiveMode;

	float yaw = 0.0f;
	float pitch = 0.0f;
	float roll = 0.0f;

	static int screenWidth;
	static int screenHeight;

};
#endif
