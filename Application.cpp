#include<Application/Application.h>

GLFWwindow* Application::window;

Application::Application(GLFWwindow* currentWin, int width, int height)
{
	BindCurrentWindow(currentWin);

	viewWidth = width;
	viewHeight = height;

	Application::InitApplication();
}

Application::~Application()
{

}

void Application::InitApplication()
{

}



