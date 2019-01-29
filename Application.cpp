#include<Application/Application.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void InitApplication()
{
	// init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);   //This is for mac OS

	// Create a new Application::window object
	Application::window = glfwCreateWindow(800, 600, "chaoGL", NULL, NULL);

	if (Application::window == NULL)
	{
		std::cout << "Failed to create GLFWwindow" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(Application::window);

	// Init GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	// Define viewport
	glViewport(0, 0, 800, 600);

	// Rigist Application::window adjust call back function 
	glfwSetFramebufferSizeCallback(Application::window, framebuffer_size_callback);
}

// Function called while change Application::window size
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

