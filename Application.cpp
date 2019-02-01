#include<Application/Application.h>

GLFWwindow* Application::window;

Application::Application(GLFWwindow* currentWin)
{
	BindCurrentWindow(currentWin);
	Application::InitApplication();
}

Application::~Application()
{

}

void Application::InitApplication()
{

}



