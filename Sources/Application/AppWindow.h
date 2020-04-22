#pragma once

#include <array>
#include <stdexcept>

struct GLFWwindow;
struct GLFWmonitor;

class AppWindow
{
public:

	AppWindow() = delete;
	AppWindow(int width, int height, const char* windowname);

	void BindLoopFunction(void (*LoopFunc)());
	GLFWwindow* Get();
	void Init(int width, int height, const char* windowname);
	void MainLoop();
	bool IsFullscreen();
	void SetFullScreen(bool fullscreen);

	int GetWidth();
	int GetHeight();
	int GetPosX();   // screen middle pos x
	int GetPosY();   // screen middle pos y

private:

	void Resize(int cx, int cy);

	static void CallbackResize(GLFWwindow* window, int cx, int cy);
	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	static void MouseCallback(GLFWwindow* window, double xpos, double ypos);


private:

	std::array< int, 2 > _wndPos{ 0, 0 };
	std::array< int, 2 > _wndSize{ 0, 0 };
	std::array< int, 2 > _vpSize{ 0, 0 };
	bool                 _updateViewport = true;
	bool                 _isFullScreen = false;
	GLFWwindow* _wnd = nullptr;
	GLFWmonitor* _monitor = nullptr;


private:

	void (*LoopFunction)() = nullptr;
};