#include<Section/Hello.h>

bool isInitialized = false;

bool enterKeyPressing = false;
bool drawTriangleMode = true;

unsigned int triVAO;
unsigned int triVBO;
unsigned int recVAO;
unsigned int recVBO;
unsigned int recEBO;

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

void Hello::HelloTriangle()
{
	if (!isInitialized)
	{
		InitTriangle();
	}

	if (glfwGetKey(Application::GetWindow(), GLFW_KEY_ENTER) == GLFW_PRESS && !enterKeyPressing)
	{
		enterKeyPressing = true;
		if (drawTriangleMode)
			drawTriangleMode = false;
		else
			drawTriangleMode = true;
	}
	if (glfwGetKey(Application::GetWindow(), GLFW_KEY_ENTER) == GLFW_RELEASE)
		enterKeyPressing = false;

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
}

void Hello::InitTriangle()
{
	//----------------------------------

	// bind triangle
	//Vertex array object
	glGenVertexArrays(1, &triVAO);
	glBindVertexArray(triVAO);	//After bind, VBO information can transfer to VAO, unbind to define other vertex
								//Vertex buffer object
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
	glGenVertexArrays(1, &recVAO);
	glBindVertexArray(recVAO);	//After bind, VBO information can transfer to VAO, unbind to define other vertex
								// Vertex buffer object
	glGenBuffers(1, &recVBO);
	glBindBuffer(GL_ARRAY_BUFFER, recVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(recVertices), recVertices, GL_STATIC_DRAW);
	//Element Buffer Object
	glGenBuffers(1, &recEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, recEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// Link vertex property
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}
