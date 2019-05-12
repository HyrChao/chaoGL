#include<Application/Application.h>

GLFWwindow* Application::window;
Application* Application::app;
Sections* Application::section;
Render* Application::render;

int Application::screenWidth;
int Application::screenHeight;
bool Application::wireframeMode;
bool Application::keyOnce[GLFW_KEY_LAST + 1];


Application::Application(GLFWwindow* currentWin, int width, int height)
{
	app = this;

	BindCurrentWindow(currentWin);

	screenWidth = width;
	screenHeight = height;
    


	Application::InitApplication();
}

Application::~Application()
{
	//delete app;
	delete section;
    delete render;
}

void Application::InitApplication()
{
    render = new Render();
    
    Mouse::SetMouseInitLocation(screenWidth,screenHeight);
    
    if (Camera::main == nullptr)
    {
        Camera::main = new Camera(screenWidth,screenHeight);
    }
    
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// set input callback
	glfwSetCursorPosCallback(window, Application::mouse_callback);
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    glViewport(0, 0, screenWidth, screenHeight);
    
    section = new Sections();
}

void Application::Update()
{
	ProcessInput();

    Render::DrawOnFrameBegin();

	// Fill mode
	if (wireframeMode)
        Render::WireframeMode(true);
	else
        Render::WireframeMode(false);

	section->SwitchSections();


	glfwSwapBuffers(Application::GetWindow());

    Render::DrawOnFrameEnd();
    Time::UpdateTime();
}





void Application::OnFrameBegin()
{
}

void Application::OnFrameEnd()
{

}

void Application::ProcessInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	// wireframe mode
	if (((glfwGetKey(window, GLFW_KEY_W)&glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) == GLFW_PRESS) && !keyOnce[GLFW_KEY_W] && !keyOnce[GLFW_KEY_LEFT_CONTROL])
	{
		keyOnce[GLFW_KEY_W] = true;
		keyOnce[GLFW_KEY_LEFT_CONTROL] = true;
		if (wireframeMode)
			wireframeMode = false;
		else
			wireframeMode = true;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
		keyOnce[GLFW_KEY_W] = false;
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
		keyOnce[GLFW_KEY_LEFT_CONTROL] = false;
	if (glfwGetKey(Application::GetWindow(), GLFW_KEY_ENTER) == GLFW_PRESS && !keyOnce[GLFW_KEY_ENTER])
	{
		keyOnce[GLFW_KEY_ENTER] = true;
		if (section->GetDefaultSection())
			section->SetDefaultSection(false);
		else
			section->SetDefaultSection(true);
	}
	if (glfwGetKey(Application::GetWindow(), GLFW_KEY_ENTER) == GLFW_RELEASE)
		keyOnce[GLFW_KEY_ENTER] = false;
	float cameraSpeed = 0.05f; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		Camera::main->MoveForward(cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		Camera::main->MoveForward(-cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		Camera::main->MoveRight(-cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		Camera::main->MoveRight(cameraSpeed);
}

void Application::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	Mouse::UpdateMouse(xpos, ypos);
	Camera::main->MoveView(Mouse::xoffset, Mouse::yoffset);
}



