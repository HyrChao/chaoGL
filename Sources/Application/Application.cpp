#include<Application/Application.h>

GLFWwindow* Application::window;
Application* Application::app;
Sections* Application::section;

bool Application::wireframeMode;

Application::Application()
{


	app = this;

	// init GLFW
	glfwInit();

	//Window
	RenderDevice::screenWidth = 800;
	RenderDevice::screenHeight = 800;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	// Mac 10.9 later using OpenGL4.1
	// need campative if using version 3.3
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);   //This is for mac OS

	// Create a new window object
	window = glfwCreateWindow(RenderDevice::screenWidth, RenderDevice::screenHeight, "chaoGL", NULL, NULL);
	glfwMakeContextCurrent(window);

	// Init GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	std::cout << "GL_Version: " << glGetString(GL_VERSION) << std::endl;

	// Rigist window adjust call back function 
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	BindCurrentWindow(window);
    
	// Frame lock
	glfwSwapInterval(1);

	Render::PrepareRender();

	Application::InitApplication();

}

Application::~Application()
{
	//delete app;
	delete section;
}

void Application::InitApplication()
{
    //render = new Render();

	if (Camera::main == nullptr)
	{
		Camera::main = new Camera();
	}
    
    Mouse::SetMouseInitLocation(RenderDevice::screenWidth, RenderDevice::screenHeight);
    

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// set input callback
	glfwSetCursorPosCallback(window, Application::mouse_callback);
    glfwGetFramebufferSize(window, &RenderDevice::screenWidth, &RenderDevice::screenHeight);
    
	Input::SetCurrentWindow(window);

	CommonAssets::instance = new CommonAssets();

    section = new Sections();

}

void Application::Update()
{
	ProcessInput();

	Render::ExcuteDrawOnFrameBegin();

	section->SwitchSections();

	Render::ExcuteDraw();

	// Fill mode
	if (wireframeMode)
        Render::WireframeMode(true);
	else
        Render::WireframeMode(false);


    Render::ExcuteDrawOnFrameEnd();

	glfwSwapBuffers(Application::GetWindow());

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Time::UpdateTime();

}





void Application::OnFrameBegin()
{
}

void Application::OnFrameEnd()
{
	// Event
	glfwPollEvents();
}

void Application::ProcessInput()
{
    Input::UpdateKeys();
    
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	// wireframe mode
	//if (((glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) == GLFW_PRESS) && Input::GetKeyOnce(GLFW_KEY_W))
	if (((glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) == GLFW_PRESS) && Input::GetKeyOnce(GLFW_KEY_W))
	{
		if (wireframeMode)
			wireframeMode = false;
		else
			wireframeMode = true;
	}

	float cameraSpeed = 0.05f; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		Camera::main->MoveForward(cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		Camera::main->MoveForward(-cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		Camera::main->MoveRight(-cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		Camera::main->MoveRight(cameraSpeed);
    
    if (Input::GetKeyOnce(GLFW_KEY_1))
        section->SetSection(SectionEnum::BlinnPhong);
    else if (Input::GetKeyOnce(GLFW_KEY_2))
        section->SetSection(SectionEnum::LoadModel);
    else if (Input::GetKeyOnce(GLFW_KEY_3))
        section->SetSection(SectionEnum::PBR);
}

void Application::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	Mouse::UpdateMouse(xpos, ypos);
	Camera::main->MoveView(Mouse::xoffset, Mouse::yoffset);
}

// Function called while change window size
void Application::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// Define viewport
	glViewport(0, 0, width, height);
}

