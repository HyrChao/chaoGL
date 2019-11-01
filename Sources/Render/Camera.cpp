#include<Render/Camera.h>

#include<Application/Application.h>


int Camera::screenHeight;
int Camera::screenWidth;

Camera * Camera::main;

Camera::Camera()
{
	SetScreenExtend(RenderDevice::screenWidth, RenderDevice::screenHeight);

	cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);

	pos = glm::vec3(0.0f, 0.0f, -3.0f);
	cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	cameraDirection = glm::normalize(pos - cameraTarget);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	cameraUp = glm::cross(cameraDirection, cameraRight);

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

void Camera::Move(glm::vec3 pos)
{
	this->pos = pos;
}

void Camera::MoveForward(float speed)
{
	pos += cameraFront * speed;
}

void Camera::MoveRight(float speed)
{
	pos += cameraRight * speed;
}

void Camera::LockAt(glm::vec3 targetPos)
{
	lookAt = true;
	cameraLockonTarget = targetPos;
}

void Camera::LockAtUnlock()
{
	lookAt = false;
}

void Camera::UpdateCamera()
{
	if (lookAt)
	{
		cameraTarget = cameraLockonTarget;
	}
	else
	{
		cameraTarget = pos + cameraFront;
	}

	// move
	cameraDirection = glm::normalize(pos - cameraTarget);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	cameraUp = glm::cross(cameraDirection, cameraRight);
	viewMat = glm::lookAt(pos, cameraTarget, cameraUp);

}

void Camera::MoveView(float yawoffset, float pitchoffset)
{

	yaw += yawoffset;
	pitch += pitchoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraFront = glm::normalize(front);

}

void Camera::CameraAutoSpan()
{
	// camera update
	float radius = 10.0f;
	float camX = sin(Time::time) * radius;
	float camZ = cos(Time::time) * radius;
	viewMat = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
}

void Camera::SetView(bool isPerspective,float in_fov, float in_nearPlane, float in_farPlane)
{
	fov = in_fov;
	nearPLane = in_nearPlane;
	farPlane = in_farPlane;
	viewProjPers = glm::perspective(glm::radians(fov), (float)screenWidth / (float)screenHeight, nearPLane, farPlane);
	viewProjOrth = glm::ortho(0.0f, (float)screenWidth, 0.0f, (float)screenHeight, 0.1f, 100.0f);
	PerspectiveMode(isPerspective);
}

void Camera::SetScreenExtend(int in_screenWidth, int in_screenHeight)
{
	screenHeight = in_screenHeight;
	screenWidth = in_screenWidth;
}

void Camera::PerspectiveMode(bool isPerspective)
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





