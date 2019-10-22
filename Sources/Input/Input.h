#pragma once
#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>

class Input
{
public:
	Input();
	~Input();

	static void Input::UpdateKeys();
	
	static bool GetKeyOnce(int key)
	{
		return keyOnce[key];
	}

	static bool GetKey(int key)
	{
		return (glfwGetKey(window, key) == GLFW_PRESS);
	}


	static void SetCurrentWindow(GLFWwindow* window)
	{
		Input::window = window;
	}


private:

	static bool keyOnce[GLFW_KEY_LAST + 1];
	static bool keyInCold[GLFW_KEY_LAST + 1];
	static GLFWwindow* window;

};


#endif
