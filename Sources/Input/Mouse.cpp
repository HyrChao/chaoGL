#include<Input/Mouse.h>
#include <GLFW/glfw3.h>
#include <Application/Application.h>

float Mouse::sensitivity = 0.05f;
float Mouse::xoffset;
float Mouse::yoffset;
float Mouse::lastX;
float Mouse::lastY;
bool Mouse::firstMouse = true;

bool Mouse::cursorToggle = true;

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

void Mouse::CursorOn(bool ison)
{
	GLFWwindow* window = Application::currentWindow->Get();
	if (ison)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		cursorToggle = true;
	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		cursorToggle = false;
	}

}

void Mouse::CursorToggle()
{
	GLFWwindow* window = Application::currentWindow->Get();
	if (cursorToggle)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		cursorToggle = false;
	}
	else 
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		cursorToggle = true;
	}
}

bool Mouse::GetCursorState()
{
	return cursorToggle;
}









