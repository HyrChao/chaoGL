// 2019-01-29 11:08:09
#pragma once
#ifndef Hello_H
#define Hello_H

#include<Render/Shader.h>
#include <GLFW/glfw3.h>

#include <chaoGL.h>

class Hello
{

public:

private:

	bool isDataInitialized = false;


	unsigned int triVAO;
	unsigned int triVBO;
	unsigned int recVAO;
	unsigned int recVBO;
	unsigned int recEBO;

	unsigned int texture1, texture2;


	Shader* helloTriShader;
	Shader* helloTexShader;
	Shader* helloProjShader;

	// Vertex init
	// triangel verts
	float triVertices[18] = {
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f
	};

	// uv coord for triangle
	float triTexCoords[6] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.5f, 1.0f
	};

	// recangle verts
	float recVertices[32] = {
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,  
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f   
	};

	// indices for recangle
	unsigned int recIndices[6] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

public:
	Hello();
	~Hello();
	void HelloTriangle();
	void LoadTexture(Shader* shader);
	void HelloTransform();
	void HelloProjection();

private:

	void Transform(Shader* shader, glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale);
	void InitData();
	void glmTest();

};

#endif // ! Hello_H
