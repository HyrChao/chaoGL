// 2019-01-29 11:08:09
#pragma once
#ifndef Hello_H
#define Hello_H

#include <glad/glad.h>
#include <Render/Render.h>
#include <Render/Color.h>
#include <Render/Model.h>
class Hello
{

public:

private:

	bool isDataInitialized = false;
    
    bool helloLightInitialized = false;
    
    bool helloModelInitialized = false;
    
    bool helloPBRInitialized = false;
    
    Model *helloModel;
    Shader *helloModelShader;
	unsigned int diffuseTex_1, diffuseTex_2, specularTex;

	Shader* helloTriShader;
	Shader* helloTexShader;
	Shader* helloProjShader;
	Shader* helloLightShader;

    Shader* helloPBRShader;
	unsigned int albedo, normal, metallic, roughness, ao;
	
	Light* pointLight1;
    Light* pointLight2;
    Light* spotLight;
    Light* dirLight;
    
	void DrawCube(Shader* shader, glm::mat4 &model)
	{
		shader->use();
		glBindVertexArray(Geo::geo->cubeVAO);
		Render::SetVertexShaderParams(shader, model);
		Render::SetShaderLightParams(shader);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}
	void DrawSphere(Shader* shader, glm::mat4 &model)
	{
		shader->use();
		Render::SetVertexShaderParams(shader, model);
		Render::SetShaderLightParams(shader);
		glBindVertexArray(Geo::geo->sphereVAO);
		glDrawElements(GL_TRIANGLE_STRIP, Geo::geo->sphereIndexCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

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

public:
	Hello();
	~Hello();
	void HelloTriangle();
	void LoadTexture(const std::string& path ,int& texId);
	void LoadTexture();
	void HelloTransform();
	void HelloProjection();
	void HelloBox();
	void HelloCamera();
    void extracted();
    
    void HelloLight();
    void HelloLightInit();
    void HelloModel();
    void HelloPBR();


private:

	void Transform(Shader* shader, glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale);
	void InitData();
	void glmTest();
	void SetupLight_Old();


};

#endif // ! Hello_H
