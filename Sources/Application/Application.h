#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include <Render/Render.h>
#include <Application/Time.h>
#include <Section/Sections.h>
#include <Input/Mouse.h>
#include<Render/Camera.h>

#include <GLFW/glfw3.h>
#include <chaoGL.h>

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
	static GLFWwindow* window;


public:

	static int screenWidth;
	static int screenHeight;

private:
	static void ProcessInput();
    static void UpdateKeys();
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

private:

	static Application* app;
	static Sections* section;
	static Render* render;

	//toggle
	static bool wireframeMode;


};
#endif // !APPLICATION_H
