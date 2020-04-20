// To use stb_image, add this in *one* C++ source file.
#define STB_IMAGE_IMPLEMENTATION

#include<Application/Application.h>
#include<Input/Input.h>

#ifdef __APPLE__
#include <mach-o/dyld.h>
#include <stdlib.h>
// getcwd() both include in <direct.h> and <unistd.h>
#elif WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

Application* app;
char const * FileSystem::app_root;

GLFWwindow* Application::window;
Application* Application::app;

bool Application::wireframeMode;
bool Application::showSystemGUI = true;

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


#ifdef WIN32
	char buffer[512];
	FileSystem::app_root = getcwd(buffer, sizeof(buffer));


#elif __APPLE__

	char path[1024];
	uint32_t size = sizeof(path);
	if (_NSGetExecutablePath(path, &size) == 0)
	{
		string directory;
		directory = realpath("chaoGL", path); // symboollink in case

		const size_t last_slash_idx = directory.rfind('/');
		if (std::string::npos != last_slash_idx)
		{
			directory = directory.substr(0, last_slash_idx);
		}
		FileSystem::app_root = directory.c_str();
		printf("[MacOS] current path is: %s\n", FileSystem::app_root);
		//FileSystem::app_root = FileSystem::GetDirectory(path).c_str();
}


	else
		printf("buffer too small; need size %u\n", size);


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
	Application::InitGUI();

}

Application::~Application()
{
	ReleaseGUI();
}

void Application::InitApplication()
{
    //render = new Render();

	if (Camera::main == nullptr)
	{
		Camera::main = new Camera();
	}
    
    Mouse::SetMouseInitLocation(RenderDevice::screenWidth, RenderDevice::screenHeight);
    
	Mouse::CursorOn(false);

	// set input callback
	glfwSetCursorPosCallback(window, Application::mouse_callback);
    glfwGetFramebufferSize(window, &RenderDevice::screenWidth, &RenderDevice::screenHeight);
    
	Input::SetCurrentWindow(window);

	CommonAssets::instance = new CommonAssets();

	LevelManager::LoadMainLevel();

}

void Application::Update()
{
	ProcessInput();

	PrepareGUIOnFrameBegin();

	Render::DrawOnFrameBegin();

	LevelManager::SceneLoop();

	Render::Draw();

	// Fill mode
	if (wireframeMode)
        Render::WireframeMode(true);
	else
        Render::WireframeMode(false);


    Render::DrawOnFrameEnd();

	DrawSystemGUI(showSystemGUI);
	LevelManager::SceneGui();


	DrawGUI();

}


void Application::OnFrameBegin()
{
}

void Application::OnFrameEnd()
{
	glfwSwapBuffers(Application::GetWindow());

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Time::UpdateTime();

	//// Event
	glfwPollEvents();
}

void Application::ProcessInput()
{
    Input::UpdateKeys();
    
	static bool cantriggercursortoggle;
	static float coldtime_cursortoggle;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT == GLFW_PRESS) && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT == GLFW_PRESS) && cantriggercursortoggle)
	{
		Mouse::CursorToggle();
		cantriggercursortoggle = false;
	}
	if (coldtime_cursortoggle > 0.5f)
	{
		cantriggercursortoggle = true;
		coldtime_cursortoggle = 0.0f;
	}
	coldtime_cursortoggle += Time::deltaTime;
	
	if (Input::GetKeyOnce(GLFW_KEY_ESCAPE))
	{
		if(Input::GetKey(GLFW_KEY_LEFT_SHIFT))
			glfwSetWindowShouldClose(window, true);

		if (showSystemGUI)
			showSystemGUI = false;
		else
			showSystemGUI = true;
	}

	

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
    




}

void Application::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (Mouse::GetCursorState())
		return;

	Mouse::UpdateMouse(xpos, ypos);
	Camera::main->MoveView(Mouse::xoffset, Mouse::yoffset);
}

// Function called while change window size
void Application::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	RenderDevice::screenHeight = height;
	RenderDevice::screenWidth = width;
	// Define viewport
	glViewport(0, 0, width, height);
}

void Application::InitGUI()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	const char* glsl_version = "#version 130";    // GL 3.0 + GLSL 130
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

}

void Application::PrepareGUIOnFrameBegin()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Application::DrawGUI()
{
	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Application::ReleaseGUI()
{
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Application::DrawSystemGUI(bool showsystemgui)
{

	static bool show_demo_window = true;
	static bool show_another_window = false;
	static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);



	if (showsystemgui)
	{

		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Sysyem Menu");
		ImGui::SameLine();
		ImGui::Text("%.3f ms/frame (%.1f FPS)", Time::deltaTime * 1000.0f, 1.0f/Time::deltaTime);

		ImGui::Text("Select Scene");

		if (ImGui::Button("BlinnPhong"))
			LevelManager::LoadLevel(LevelName::BlinnPhong);
		if (ImGui::Button("LoadModel"))
			LevelManager::LoadLevel(LevelName::LoadModel);
		if (ImGui::Button("PBR"))
			LevelManager::LoadLevel(LevelName::PBR);
		if (ImGui::Button("Shadow"))
			LevelManager::LoadLevel(LevelName::Shadow);

		if (ImGui::Button("Quit"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			glfwSetWindowShouldClose(window, true);

		ImGui::End();



		//static float f = 0.0f;
		//static int counter = 0;

		//ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		//ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		//ImGui::Checkbox("Another Window", &show_another_window);

		//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		//if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		//	counter++;
		//ImGui::SameLine();
		//ImGui::Text("counter = %d", counter);

		//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		//ImGui::End();

	}
}


int Close();

int main()
{


	// App & Sections
	app = new Application();



	//-------------------------------------------------------------
	// Loop
	while (!glfwWindowShouldClose(Application::GetWindow()))
	{
		// Input
		// Check if hit esc key

		// Render
		Application::OnFrameBegin();

		Application::Update();

		// on ftame end
		Application::OnFrameEnd();
	}

	return Close();

}


int Close()
{
	//Release resource while terminate
	glfwTerminate();
	delete app;

	return 0;
}