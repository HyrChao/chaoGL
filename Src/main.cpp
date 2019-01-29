//  Chao 19/101/27
// https://learnopengl-cn.github.io/01%20Getting%20started/03%20Hello%20Window/

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Render/Shader.h>

#include <chaoGL.h>
#include <Section/Sections.h>

void processInput(GLFWwindow *window);

// Shader content
bool wireframeMode = false;
bool wKeyPressing = false;

//GLFWwindow* window;

int main()
{
	InitApplication();
	//Shader
	Shader shader("./Shader/Vertex/HelloTriangle.vs", "./Shader/Fragment/HelloTriangle.fs");
	
	//-------------------------------------------------------------
	// Loop
	while (!glfwWindowShouldClose(Application::window))
	{
		// Input
		// Check if hit esc key
		processInput(Application::window);

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


		Sections::HelloTriangle();

		// Event
		glfwSwapBuffers(Application::window);
		glfwPollEvents();
	}

	//Release resource while terminate
	glfwTerminate();

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
