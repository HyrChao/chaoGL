#pragma once
#ifndef COMMONASSETS_H
#define COMMONASSETS_H

#include <glad/glad.h>
//#include <Render/Shader.h>
//#include <Object/Transform.h>
#include <glm.hpp>
#include <vector>
#include <iostream>

#include <Render/Shader.h>
#include <Assets/AssetsManager.h>

class Model;

class CommonAssets
{
public:
	CommonAssets();
	~CommonAssets();

	// vertex buffer
	static CommonAssets* instance;
	unsigned int triVAO;
	unsigned int triVBO;
	unsigned int recVAO;
	unsigned int recVBO;
	unsigned int recEBO;
	unsigned int cubeVAO;
	unsigned int cubeVBO;
	unsigned int sphereVAO;
	unsigned int sphereVBO;
	unsigned int sphereEBO;
	unsigned int sphereIndexCount;

	// model assets
	unique_ptr<Model> basicGeo_Cone = nullptr;
	unique_ptr<Model> basicGeo_Cube = nullptr;
	unique_ptr<Model> basicGeo_Cylinder = nullptr;
	unique_ptr<Model> basicGeo_Helix = nullptr;
	unique_ptr<Model> basicGeo_Pipe = nullptr;
	unique_ptr<Model> basicGeo_Prism = nullptr;
	unique_ptr<Model> basicGeo_Pyramid = nullptr;
	unique_ptr<Model> basicGeo_Sphere = nullptr;
	unique_ptr<Model> basicGeo_Torus = nullptr;

	// texture
	unsigned int blackTex, whiteTex, greyTex;
	unsigned int flatNormal;

	// buffer
	unsigned int backBuffer;
	unsigned int depthBuffer;

	// shader
	Shader* defaltErrorShader;
	Shader* standardPBRShader;
	Shader* drawGBufferShader;

	glm::mat4* defaltModelMat;

public:

	static void DrawPlane();

	static void DrawQuad()
	{
		glBindVertexArray(CommonAssets::instance->recVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	static void DrawCube()
	{
		glBindVertexArray(CommonAssets::instance->cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}
	static void DrawSphere()
	{
		glBindVertexArray(CommonAssets::instance->sphereVAO);
		glDrawElements(GL_TRIANGLE_STRIP, CommonAssets::instance->sphereIndexCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
private:

	void LoadCommonTextures();
	void LoadSphere();
	void LoadCube();
	void LoadTriangle();
	void LoadRecangle();
	void LoadShaders();
	void LoadBasicGeos();

private:



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
	float recVertices[20] = {
		-1.0f,  -1.0f, 0.0f,  0.0f, 0.0f,
		1.0f,  -1.0f, 0.0f,   1.0f, 0.0f,
		1.0f,  1.0f, 0.0f,   1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f,  0.0f, 1.0f
	};

	// indices for recangle
	unsigned int recIndices[6] = {
		0, 1, 2, // first triangle
		0, 3, 2  // second triangle
	};
	// cube verts
	float cubeVertices[288] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
	};

	glm::mat4 defaultModelMat_Val;

};


#endif //!COMMONASSETS_H
