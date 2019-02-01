//  Chao 19/101/27
// https://learnopengl-cn.github.io/01%20Getting%20started/03%20Hello%20Window/

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Render/Shader.h>
#include <chaoGL.h>
#include <Section/Sections.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
int Close();

// Shader content
bool wireframeMode = false;
bool wKeyPressing = false;

//GLFWwindow* window;
GLFWwindow* window;
Application* app;
Sections* sections;

int main()
{
	// init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);   //This is for mac OS

	//Window
	// Create a new window object
	window = glfwCreateWindow(800, 600, "chaoGL", NULL, NULL);
	glfwMakeContextCurrent(window);

	app = new Application(window);
	// Init GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Define viewport
	glViewport(0, 0, 800, 600);

	// Rigist window adjust call back function 
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Sections
	sections = new Sections();
	//Shader
	Shader shader("./Shader/Vertex/HelloTriangle.vs", "./Shader/Fragment/HelloTriangle.fs");
	
	//-------------------------------------------------------------
	// Loop
	while (!glfwWindowShouldClose(Application::GetWindow()))
	{
		// Input
		// Check if hit esc key
		processInput(Application::GetWindow());

		// Render
		// Use shader program
		shader.use();
		// Update color with time
		float timeValue = glfwGetTime();
		float redVal = (cos(timeValue) / 2.0f) + 0.5f;
		float greenVal = (cos(timeValue + 3.14/3) / 2.0f) + 0.5f;
		float blueVal = (cos(timeValue - 3.14/3) / 2.0f) + 0.5f;
		shader.setVec4f("ourColor",redVal,greenVal,blueVal,1.0f);
		shader.use();
		//// Clear test
		glClearColor(0.8f+0.2*sin(timeValue), 0.8f+0.2*sin(timeValue+3.14/3), 0.8f+0.2*sin(timeValue - 3.14 / 3), 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// Fill mode
		if (wireframeMode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


		sections->HelloTriangle();

		// Event
		glfwSwapBuffers(Application::GetWindow());
		glfwPollEvents();
	}

	return Close();

}


int Close()
{
	//Release resource while terminate
	glfwTerminate();
	delete app;
	delete sections;

	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	// wireframe mode
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && !wKeyPressing)
	{
		wKeyPressing = true;
		if (wireframeMode)
			wireframeMode = false;
		else
			wireframeMode = true;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
		wKeyPressing = false;
}

// Function called while change window size
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
