#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include <GLFW/glfw3.h>
#include <Application/Render.h>
#include <iostream>

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

public:

	static int screenWidth;
	static int screenHeight;


private:

	static GLFWwindow* window;
	static Application* app;

};
#endif // !APPLICATION_H
