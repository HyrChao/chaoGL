#include "AppWindow.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <glad/glad.h>
#include <Input/Mouse.h>
#include <Render/Camera.h>
#include <Render/Camera.h>

AppWindow::AppWindow(int width, int height, const char* windowname)
{
    Init(width, height, windowname);
}

void AppWindow::Init(int width, int height, const char* windowname )
{
    _wnd = glfwCreateWindow(width, height,windowname , nullptr, nullptr);
    if (_wnd == nullptr)
    {
        glfwTerminate();
        throw std::runtime_error("error initializing window");
    }

    glfwMakeContextCurrent(_wnd);

    glfwSetWindowUserPointer(_wnd, this);
    glfwSetWindowSizeCallback(_wnd, AppWindow::CallbackResize);
    // Rigist window adjust call back function 
    glfwSetFramebufferSizeCallback(_wnd, AppWindow::FramebufferSizeCallback);
    // set input callback
    glfwSetCursorPosCallback(_wnd, AppWindow::MouseCallback);
    _monitor = glfwGetPrimaryMonitor();
    glfwGetWindowSize(_wnd, &_wndSize[0], &_wndSize[1]);
    glfwGetWindowSize(_wnd, &_vpSize[0], &_vpSize[1]);
    glfwGetWindowPos(_wnd, &_wndPos[0], &_wndPos[1]);
    _updateViewport = true;
}

void AppWindow::MainLoop()
{
    while (!glfwWindowShouldClose(_wnd))
    {
        if (_updateViewport)
        {
            if (!_isFullScreen)
            {
                glfwGetWindowPos(_wnd, &_wndPos[0], &_wndPos[1]);
            }
            glfwGetFramebufferSize(_wnd, &_vpSize[0], &_vpSize[1]);
            // in case view port size get too small
            if (_vpSize[0] < 2 || _vpSize[1] < 2)
            {
                _vpSize[0] = _vpSize[1] = 2;
            }
            glViewport(0, 0, _vpSize[0], _vpSize[1]);
            Camera::main->UpdateScreenExtend(_vpSize[0], _vpSize[1]);
            _updateViewport = false;
        }

        //  render the scene
        LoopFunction();

        glfwSwapBuffers(_wnd);
        //// Event
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

bool AppWindow::IsFullscreen()
{
    return _isFullScreen;
}

void AppWindow::SetFullScreen(bool fullscreen)
{
    // get current fullscreen state 
    _isFullScreen = (glfwGetWindowMonitor(_wnd) != nullptr);
    if (_isFullScreen == fullscreen)
        return;

    if (fullscreen)
    {
        // backup window position and window size
        glfwGetWindowPos(_wnd, &_wndPos[0], &_wndPos[1]);
        glfwGetWindowSize(_wnd, &_wndSize[0], &_wndSize[1]);
        // get resolution of monitor
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        // switch to full screen
        glfwSetWindowMonitor(_wnd, _monitor, 0, 0, mode->width, mode->height, 0);
        _isFullScreen = true;
    }
    else
    {
        // restore last window size and position
        glfwSetWindowMonitor(_wnd, nullptr, _wndPos[0], _wndPos[1], _wndSize[0], _wndSize[1], 0);
        _isFullScreen = false;
    }

    _updateViewport = true;
}

int AppWindow::GetWidth()
{
    return _vpSize[0];
}

int AppWindow::GetHeight()
{
    return _vpSize[1];
}

int AppWindow::GetPosX()
{
    int x = _wndPos[0];
    if (_isFullScreen)
        x = 0;
    return x + int(_vpSize[0] / 2);
}

int AppWindow::GetPosY()
{
    int y = _wndPos[1];
    if (_isFullScreen)
        y = 0;
    return y + int(_vpSize[1] / 2);
}

void AppWindow::Resize(int cx, int cy)
{
    if(!_isFullScreen)
        _updateViewport = true;
}

void AppWindow::CallbackResize(GLFWwindow* window, int cx, int cy)
{
    void* ptr = glfwGetWindowUserPointer(window);
    if (AppWindow* wndPtr = static_cast<AppWindow*>(ptr))
        wndPtr->Resize(cx, cy);
}

// Function called while change window size
void AppWindow::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // Define viewport
    //glViewport(0, 0, width, height);
}

void AppWindow::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (Mouse::GetCursorState())
        return;

    Mouse::UpdateMouse(xpos, ypos);
    Camera::main->MoveView(Mouse::xoffset, Mouse::yoffset);
}


void AppWindow::BindLoopFunction(void(*LoopFunc)())
{
    LoopFunction = LoopFunc;
}

GLFWwindow* AppWindow::Get()
{
    if (_wnd == nullptr)
    {
        std::cout << "Failed to create GLFWwindow" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    else
    {
        return _wnd;
    }
}