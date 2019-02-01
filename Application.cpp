#include<Application/Application.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

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



