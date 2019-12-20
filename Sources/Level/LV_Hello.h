// 2019-01-29 11:08:09
#pragma once
#ifndef LV_Hello_H
#define LV_Hello_H

#include <glad/glad.h>
#include <Render/Render.h>
#include <Render/Color.h>
#include <Render/Model.h>

#include<Input/Input.h>
#include "Level/Level.h"
#include <memory>

class LV_Hello : public Level
{

public:
	LV_Hello();
	~LV_Hello();
	void HelloTriangle();
	void LoadTexture(const std::string& path ,int& texId);
	void HelloTransform();
	void HelloProjection();
	void HelloBox();
	void HelloCamera();
    void extracted();
    
    void HelloLight();
    void HelloLightInit();

	void Reset()
	{
		helloLightInitialized = false;
		helloModelInitialized = false;
	}

	void Loop() override;
	void OnGui() override;

protected:

	void Initialize() override;

public:

private:

	void Transform(Shader* shader, glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale);
	void InitData();
	void glmTest();
	void SetupLight_Old();

	void LoadTexture();

	void SetShaderLightParams(Shader * shader);


private:

	bool helloLightInitialized = false;

	bool helloModelInitialized = false;


	unique_ptr<Shader> helloTriShader;
	unique_ptr<Shader> helloTexShader;
	unique_ptr<Shader> helloProjShader;
	unique_ptr<Shader> helloLightShader;


	Light* pointLight1;
	Light* pointLight2;
	Light* spotLight;
	Light* dirLight;

	unsigned int specularTex, diffuseTex_1, diffuseTex_2;


	glm::vec3 lightCol;

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

private:

	enum Hello_Scene
	{
		S_HelloLight,
		S_HelloTriangle,
		S_HelloTransform,
		S_HelloProjection,
		S_HelloBox,
		S_HelloCamera,
		LastScene
	};

	unsigned int currentHelloScene = S_HelloLight;

};
#endif // ! LV_Hello_H
