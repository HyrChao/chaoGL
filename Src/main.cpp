//  Chao 19/101/27
// https://learnopengl-cn.github.io/01%20Getting%20started/03%20Hello%20Window/

#include <Application/Application.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
int Close();



//GLFWwindow* window;
GLFWwindow* window;
Application* app;

int windowWidth;
int windowHeight;

int main()
{
	// init GLFW
	glfwInit();

	//Window
	windowWidth = 800;
	windowHeight = 800;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);   //This is for mac OS
	// Create a new window object
	window = glfwCreateWindow(windowWidth, windowHeight, "chaoGL", NULL, NULL);
	glfwMakeContextCurrent(window);

	// Init GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Rigist window adjust call back function 
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// App & Sections
	app = new Application(window,windowWidth,windowHeight);


	
	//-------------------------------------------------------------
	// Loop
	while (!glfwWindowShouldClose(Application::GetWindow()))
	{
		// Input
		// Check if hit esc key

		// Render
		Application::OnFrameBegin();

		Application::Update();
		// Event
		glfwPollEvents();

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

// Function called while change window size
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// Define viewport
	glViewport(0, 0, width, height);
}
