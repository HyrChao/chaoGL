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

AppWindow* Application::currentWindow;
Application* Application::app;

bool Application::wireframeMode;
bool Application::showSystemGUI = true;

Application::Application()
{

	app = this;


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


	Application::InitApplication();
	Application::InitGUI();

}

Application::~Application()
{
	ReleaseGUI();
}

void Application::InitApplication()
{
	// init GLFW
	glfwInit();

	//Window
	int screenWidth = 800;
	int screenHeight = 800;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a new window object
	currentWindow = new AppWindow(screenWidth, screenHeight, "ChaoGL");

	// Init GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	std::cout << "GL_Version: " << glGetString(GL_VERSION) << std::endl;


	currentWindow->BindLoopFunction(Application::Update);

	// Frame lock
	glfwSwapInterval(1);

	Mouse::SetMouseInitLocation(currentWindow->GetPosX(), currentWindow->GetPosY());
	Mouse::CursorOn(false);

	Render::PrepareRender();

	if (Camera::main == nullptr)
	{
		Camera::main = new Camera();
	}

	Input::SetInputTargetWindow(currentWindow->Get());

	CommonAssets::instance = new CommonAssets();

	LevelManager::LoadMainLevel();

}

void Application::Update()
{
	OnFrameBegin();

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

	if (showSystemGUI)
	{
		ImGui::Begin("Menu");
		ImGui::NewLine();
		ImGui::SameLine();
		SystemGUI();
		ImGui::NewLine();
		ImGui::SameLine();
		LevelManager::SceneGui();
		ImGui::End();
	}


	DrawGUI();

	OnFrameEnd();
}


void Application::OnFrameBegin()
{

}

void Application::OnFrameEnd()
{

	Time::UpdateTime();

}

inline void Application::BindCurrentWindow(AppWindow* wind)
{
	currentWindow = wind;
}

void Application::ProcessInput()
{
	Input::SetInputTargetWindow(currentWindow->Get());
    Input::UpdateKeys();

	GLFWwindow* window = currentWindow->Get();
	static bool cantriggercursortoggle;
	static float coldtime_cursortoggle;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT == GLFW_PRESS))
	{
		Mouse::SetDrag(true);
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT == GLFW_PRESS) && cantriggercursortoggle)
		{
			Mouse::CursorToggle();
			cantriggercursortoggle = false;
		}
		else;
	}
	Mouse::SetDrag(false);

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

	if (Input::GetKeyOnce(GLFW_KEY_F11))
		if (currentWindow->IsFullscreen())
			currentWindow->SetFullScreen(false);
		else
			currentWindow->SetFullScreen(true);
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
	ImGui_ImplGlfw_InitForOpenGL(currentWindow->Get(), true);
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

void Application::SystemGUI()
{

	static bool show_demo_window = true;
	static bool show_another_window = false;
	static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	{

		static float f = 0.0f;
		static int counter = 0;

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
		if (ImGui::Button("PostFX"))
			LevelManager::LoadLevel(LevelName::PostFX);

		if (ImGui::Button("Quit"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			glfwSetWindowShouldClose(currentWindow->Get(), true);




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

// Main 
int main()
{
	// App & Sections
	app = new Application();

	//-------------------------------------------------------------;
	app->currentWindow->MainLoop();

	return Close();

}


int Close()
{
	//Release resource while terminate
	glfwTerminate();
	delete app;

	return 0;
}