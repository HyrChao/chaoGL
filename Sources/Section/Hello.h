// 2019-01-29 11:08:09
#pragma once
#ifndef Hello_H
#define Hello_H

#include <glad/glad.h>
#include <Render/Light.h>

class Hello
{

public:

private:

	bool isDataInitialized = false;

	unsigned int diffuseTex, specularTex, texture1, texture2;

	Shader* helloTriShader;
	Shader* helloTexShader;
	Shader* helloProjShader;
	Shader* helloLightShader;

	Light* light1;

	glm::vec3 cubePositions[10] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

public:
	Hello();
	~Hello();
	void HelloTriangle();
	void LoadTexture(const std::string& path ,int& texId);
	void LoadTexture(Shader* shader);
	void HelloTransform();
	void HelloProjection();
	void HelloBox();
	void HelloCamera();
	void HelloLight();


private:

	void Transform(Shader* shader, glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale);
	void InitData();
	void glmTest();
	void SetupLight();

};

#endif // ! Hello_H
