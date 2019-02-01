#include<Application/Application.h>

GLFWwindow* Application::window;

Application::Application(GLFWwindow* currentWin, int width, int height)
{
	BindCurrentWindow(currentWin);

	viewWidth = width;
	viewHeight = height;

	// set uv repeat mode, S and T direction
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	// set filtering for zoom in / zoom out
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// mipmap for zoom out
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



	Application::InitApplication();


}

Application::~Application()
{

}

void Application::InitApplication()
{

}



