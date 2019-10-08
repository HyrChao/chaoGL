#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

//#include <GLFW/glfw3.h>
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
    static bool GetKeyOnce(int key)
    {
        return keyOnce[key];
    }

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
	//static bool wKeyPressing;
	//static bool enterKeyPressing;
	static bool keyOnce[GLFW_KEY_LAST + 1];
	static bool keyInCold[GLFW_KEY_LAST + 1];


};
#endif // !APPLICATION_H
