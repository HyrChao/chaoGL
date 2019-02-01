#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Application
{
public:

	Application(GLFWwindow* currentWin, int width, int height);
	~Application();

	static void InitApplication();

	static void BindCurrentWindow(GLFWwindow* currentWin)
	{
		window = currentWin;
	}

	static GLFWwindow* GetWindow()
	{
		if (window == nullptr)
		{
			std::cout << "Failed to create GLFWwindow" << std::endl;
			glfwTerminate();
			return nullptr;
		}
		else
		{
			return window;
		}
	}

private:

	static GLFWwindow* window;

	int viewWidth;
	int viewHeight;

};

//namespace Application
//{
//}
#endif // !APPLICATION_H
