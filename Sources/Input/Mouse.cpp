#include<Input/Mouse.h>
#include <GLFW/glfw3.h>
#include <Application/Application.h>

float Mouse::sensitivity = 0.05f;
float Mouse::xoffset;
float Mouse::yoffset;
glm::vec4 Mouse::mouseParams;
bool Mouse::firstMouse = true;

bool Mouse::cursorToggle = true;

void Mouse::UpdateMouse(float xpos, float ypos)
{
	if (firstMouse)
	{
		mouseParams.x = xpos;
		mouseParams.y = ypos;
		firstMouse = false;
	}

	xoffset = xpos - mouseParams.x;
	yoffset = mouseParams.y - ypos;
	mouseParams.x = xpos;
	mouseParams.y = ypos;

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
	mouseParams.x = screemWidth/2;
	mouseParams.y = screemHeight/2;
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

void Mouse::SetDrag(bool on)
{
	if (on)
	{
		mouseParams.z = 1.0;
	}
	else
	{
		mouseParams.z = 0.0;
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

glm::vec4 Mouse::GetMouseParameter()
{
	return mouseParams;
}









