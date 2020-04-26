#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include <Render/Render.h>
#include <Application/Time.h>
#include <Level/LevelManager.h>
#include <Input/Mouse.h>
#include<Render/Camera.h>
#include <GLFW/glfw3.h>
#include <chaoGL.h>

#include <Application/AppWindow.h>

#include <iostream>
#include <cmath>


class Application
{
public:

	Application();
	~Application();

	static void InitApplication();
	static void Update();
	static void OnFrameBegin();
	static void OnFrameEnd();
	static void BindCurrentWindow(AppWindow* currentWin);

	static AppWindow* currentWindow;


public:

private:
	static void ProcessInput();
    static void UpdateKeys();
	static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void InitGUI();
	static void PrepareGUIOnFrameBegin();
	static void DrawGUI();
	static void ReleaseGUI();
	static void SystemGUI();

private:

	static Application* app;

	//toggle
	static bool showSystemGUI;
	static bool wireframeMode;


};
#endif // !APPLICATION_H
