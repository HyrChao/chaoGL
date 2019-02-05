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

	section = new Sections();
	render = new Render();

	if (Camera::main == nullptr)
	{
		Camera::main = new Camera(width,height);
	}

	BindCurrentWindow(currentWin);

	screenWidth = width;
	screenHeight = height;

	Application::InitApplication();
}

Application::~Application()
{
	//delete app;
	delete section;
}

void Application::InitApplication()
{

}

void Application::Update()
{
	ProcessInput();

	render->DrawOnFrameBegin();

	// Fill mode
	if (wireframeMode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	Camera::main->UpdateCamera();
	section->SwitchSections();


	glfwSwapBuffers(Application::GetWindow());

	render->DrawOnFrameEnd();
}

void Application::OnFrameBegin()
{
}

void Application::OnFrameEnd()
{
	Time::UpdateTime();
}

void Application::ProcessInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	// wireframe mode
	if (((glfwGetKey(window, GLFW_KEY_W)&glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) == GLFW_PRESS)&&!keyOnce[GLFW_KEY_W]&&!keyOnce[GLFW_KEY_LEFT_CONTROL])
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



