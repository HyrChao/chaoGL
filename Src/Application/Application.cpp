#include<Application/Application.h>

GLFWwindow* Application::window;
Application* Application::app;

int Application::screenWidth;
int Application::screenHeight;

Application::Application(GLFWwindow* currentWin, int width, int height)
{
	app = this;

	BindCurrentWindow(currentWin);

	screenWidth = width;
	screenHeight = height;



	Application::InitApplication();
}

Application::~Application()
{
	//delete app;
}

void Application::InitApplication()
{

}



