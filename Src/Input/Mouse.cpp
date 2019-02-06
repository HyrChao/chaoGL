#include<Input/Mouse.h>

float Mouse::sensitivity = 0.05f;
float Mouse::xoffset;
float Mouse::yoffset;
float Mouse::lastX;
float Mouse::lastY;
bool Mouse::firstMouse = true;

void Mouse::UpdateMouse(float xpos, float ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	xoffset = xpos - lastX;
	yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
}

void Mouse::SetSensitivity(float sensi)
{
	sensitivity = sensi;
}

void Mouse::SetMouseInitLocation(int screemWidth, int screemHeight)
{
	lastX = screemWidth/2;
	lastY = screemHeight/2;
}









