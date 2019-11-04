#pragma once
#ifndef RENDER_H
#define RENDER_H

//#include <GLFW/glfw3.h>
//#include <Render/Shader.h>
//#include <Render/Texture.h>
#include <Render/Camera.h>
#include <Render/Light.h>
#include <Render/Capture.h>
#include <Render/Shadow.h>

//#include <Render/Material.h>

class Render
{
public:

	Render();
	~Render();
	static void SetupRenderProperty();
	static void DrawOnFrameBegin();
	static void DrawOnFrameEnd();

	static void UpdateShaderLightParams()
	{
		for (unordered_map<int, Shader*>::iterator it = Shader::loadedShaders.begin(); it != Shader::loadedShaders.end(); it++)
		{
			Shader* shader = it->second;
			if (!shader->isProp)
				continue;
			shader->use();
			list<Light*>::iterator i = Light::lights.begin();
			int pointLightNum = 0;
			while (i != Light::lights.end())
			{
				Light *light = *i;
				if (light->type == Light::LightType::Directional)
				{
					shader->setVec3f("dirLight.direction", light->dir);
					shader->setVec3f("dirLight.ambient", 0.02f * light->color);
					shader->setVec3f("dirLight.diffuse", 0.5f * light->color);
					shader->setVec3f("dirLight.specular", 1.0f * light->color);
					shader->setVec3f("dirLight.irradiance", 5.0f * light->color);
				}
				else if (light->type == Light::LightType::Point && pointLightNum < Light::maxPointLight)
				{
					std::string num = std::to_string(pointLightNum);
					shader->setVec3f("pointLights[" + num + "].position", light->GetPos());
					shader->setFloat("pointLights[" + num + "].constant", light->constant);
					shader->setFloat("pointLights[" + num + "].linear", light->linear);
					shader->setFloat("pointLights[" + num + "].quadratic", light->quadratic);
					shader->setVec3f("pointLights[" + num + "].ambient", 0.01f * light->color);
					shader->setVec3f("pointLights[" + num + "].diffuse", 0.5f * light->color);
					shader->setVec3f("pointLights[" + num + "].specular", 0.8f * light->color);
					shader->setVec3f("pointLights[" + num + "].irradiance", 5.0f * light->color);

					pointLightNum++;
				}
				else if (light->type == Light::LightType::Spot)
				{
					shader->setVec3f("spotLight.position", light->GetPos());
					shader->setVec3f("spotLight.direction", light->dir);
					shader->setFloat("spotLight.cutOff", light->cutOff);  // cutoff is cosine of angle
					shader->setFloat("spotLight.outerCutOff", light->outerCutOff);
					shader->setVec3f("spotLight.ambient", 0.01f * light->color);
					shader->setVec3f("spotLight.diffuse", 0.8f * light->color);
					shader->setVec3f("spotLight.specular", 0.5f * light->color);
					shader->setVec3f("spotLight.irradiance", 5.0f * light->color);
				}
				else
				{

				}

				i++;

			}
		}
	}

	static void UpdateShaderCameraVP()
	{
		for (unordered_map<int, Shader*>::iterator it = Shader::loadedShaders.begin(); it != Shader::loadedShaders.end(); it++)
		{
			Shader* shader = it->second;

			if (!shader->isProp)
				continue;

			shader->use();
			shader->setMat4f("view", viewMat);
			shader->setMat4f("projection", projectMat);
			shader->setVec3f("viewPos", viewPos);
		}
	}

	static void SetMaterialCameraVP(Material* material)
	{
		material->SetParam("view", viewMat);
		material->SetParam("projection", projectMat);
		material->SetParam("viewPos", viewPos);
	}

    static void SetVertexShaderParams(Shader *shader, glm::mat4 model = glm::mat4(1.0))
    {
        shader->setMat4f("view", viewMat);
        shader->setMat4f("projection", projectMat);
        shader->setVec3f("viewPos", viewPos);
        shader->setMat4f("model", model);
    }


    static void SetShaderLightParams(Shader* shader);
    static void SetClearColor(glm::vec4 color)
    {
        clearColor = color;
    }
    static void WireframeMode(bool on = false)
    {
        if (on)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }


private:
    
    static void UpdateLight();
    
public:

	static Render* render;
    static glm::mat4 projectMat;
    static glm::mat4 viewMat;
    static glm::vec3 viewPos;

	static void DrawGeo(unsigned int geoVAO, Shader* shader, glm::mat4 &model)
	{
		shader->use();
		Render::SetVertexShaderParams(shader, model);
		Render::SetShaderLightParams(shader);
		glBindVertexArray(geoVAO);
		glDrawElements(GL_TRIANGLE_STRIP, geoVAO, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}



	static void DrawCube(Shader* shader, glm::mat4 &model)
	{
		shader->use();
		glBindVertexArray(CommonAssets::instance->cubeVAO);
		Render::SetVertexShaderParams(shader, model);
		Render::SetShaderLightParams(shader);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}

	static void DrawSphere(Shader* shader, glm::mat4 &model)
	{
		shader->use();
		Render::SetVertexShaderParams(shader, model);
		Render::SetShaderLightParams(shader);
		glBindVertexArray(CommonAssets::instance->sphereVAO);
		glDrawElements(GL_TRIANGLE_STRIP, CommonAssets::instance->sphereIndexCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

    
	// Debug frame buffer texture
	static void DisplayFramebufferTexture(Texture texture)
	{
		unsigned int textureID = texture.id;

		if (!framebufferDebugInitialized)
		{
			framebufferDebugShader = new Shader("/Shaders/Debug/Debug_Framebuffer.vs", "/Shaders/Debug/Debug_Framebuffer.fs");
			
			framebufferDebugInitialized = true;
		}
		glm::vec2 anchor = glm::vec2(0.8, 0.8);
		glm::vec2 scale = glm::vec2(0.3);
		glActiveTexture(GL_TEXTURE0);
		framebufferDebugShader->use();
		framebufferDebugShader->setVec2f("anchor", anchor);
		framebufferDebugShader->setVec2f("scale", scale);
		//framebufferDebugShader->setMat4f("view", viewMat);
		//framebufferDebugShader->setMat4f("projection", projectMat);
		framebufferDebugShader->setInt("bufferTex", 0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		CommonAssets::DrawQuad();
		glUseProgram(0);
	}

private:

    static glm::vec4 clearColor;


	static Shader* framebufferDebugShader;
	static bool framebufferDebugInitialized;

};
#endif
