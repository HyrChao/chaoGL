//  Chao 19/101/27
// https://learnopengl-cn.github.io/01%20Getting%20started/03%20Hello%20Window/

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Material/Shader.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// Shader content
bool drawTriangleMode = true;
bool wireframeMode = false;
bool enterKeyPressing = false;
bool wKeyPressing = false;
int main()
{
	// init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);   //This is for mac OS

	// Create a new window object
	GLFWwindow* window = glfwCreateWindow(800, 600, "chaoGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

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
	
	//----------------------------------
	// Vertex init
	// triangel verts
	float triVertices[] = {
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f
	};
	// recangle verts
	float recVertices[] = {
		//// first triangle
		//0.5f, 0.5f, 0.0f,   
		//0.5f, -0.5f, 0.0f,  
		//-0.5f, 0.5f, 0.0f,  
		//// second triangle			
		//0.5f, -0.5f, 0.0f,  
		//-0.5f, -0.5f, 0.0f, 
		//-0.5f, 0.5f, 0.0f
		0.5f, 0.5f, 0.0f,   
		0.5f, -0.5f, 0.0f,  
		-0.5f, -0.5f, 0.0f, 
		-0.5f, 0.5f, 0.0f   
	};
	// indices for recangle
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	// bind triangle
	//Vertex array object
	unsigned int triVAO;
	glGenVertexArrays(1, &triVAO);
	glBindVertexArray(triVAO);	//After bind, VBO information can transfer to VAO, unbind to define other vertex
	//Vertex buffer object
	unsigned int triVBO;
	glGenBuffers(1, &triVBO);
	glBindBuffer(GL_ARRAY_BUFFER, triVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triVertices), triVertices, GL_STATIC_DRAW);
	// vertex pos property
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// vertex color property
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// bind recangle
	//Vertex array object
	unsigned int recVAO;
	glGenVertexArrays(1, &recVAO);
	glBindVertexArray(recVAO);	//After bind, VBO information can transfer to VAO, unbind to define other vertex
	// Vertex buffer object
	unsigned int recVBO;
	glGenBuffers(1, &recVBO);
	glBindBuffer(GL_ARRAY_BUFFER, recVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(recVertices), recVertices, GL_STATIC_DRAW);
	//Element Buffer Object
	unsigned int recEBO;
	glGenBuffers(1, &recEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, recEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// Link vertex property
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Shader
	Shader shader("./Shader/Vertex/HelloTriangle.vs", "./Shader/Fragment/HelloTriangle.fs");
	//-------------------------------------------------------------
	// Loop
	while (!glfwWindowShouldClose(window))
	{
		// Input
		// Check if hit esc key
		processInput(window);

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
		if (drawTriangleMode)
		{
			//Triangle draw test
			glBindVertexArray(triVAO);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			glBindVertexArray(0);
		}
		else 
		{
			//Recangle draw test
			glBindVertexArray(recVAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //6 is total vertex count, use get length is a better idea
			glBindVertexArray(0);
		}

		// Event
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Release resource while terminate
	glfwTerminate();

	return 0;
}

// Function called while change window size
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && !enterKeyPressing) 
	{
		enterKeyPressing = true;
		if (drawTriangleMode)
			drawTriangleMode = false;
		else
			drawTriangleMode = true;
	}
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE)
		enterKeyPressing = false;

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
