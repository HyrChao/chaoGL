#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

//#include <GLFW/glfw3.h>
#include <Application/Render.h>
#include <Application/Time.h>
#include <Section/Sections.h>
//#include <Application/Time.h>
//#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb/stb_image.h"
#include <File/filesystem.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>


class Application
{
public:

	Application(GLFWwindow* currentWin, int width, int height);
	~Application();

	static void InitApplication();
	static void Update();
	static void OnFrameBegin();
	static void OnFrameEnd();
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
	static void ProcessInput();

private:

	static GLFWwindow* window;
	static Application* app;
	static Sections* section;
	static Render* render;

	//toggle
	static bool wireframeMode;
	//static bool wKeyPressing;
	//static bool enterKeyPressing;
	static bool keyOnce[GLFW_KEY_LAST + 1];


};
#endif // !APPLICATION_H
