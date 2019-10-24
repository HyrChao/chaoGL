#pragma once
#ifndef RENDER_H
#define RENDER_H

//#include <GLFW/glfw3.h>
#include <glad/glad.h>
//#include <Render/Shader.h>
//#include <Render/Texture.h>
#include <Render/Camera.h>
#include <Render/Light.h>
//#include <Render/Material.h>

class Render
{
public:

	Render(int screenWidth, int screenHeight);
	~Render();
	static void SetupRenderProperty();
	static void DrawOnFrameBegin();
	static void DrawOnFrameEnd();
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

	static void SetViewport(int screenWidth, int screenHeight)
	{
		glViewport(0, 0, screenWidth, screenHeight);
	}
	static void ResetViewport()
	{
		glViewport(0, 0, screenWidth, screenHeight);
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

	static void DrawPlane();

	static void DrawQuad()
	{
		glBindVertexArray(CommonAssets::instance->recVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
	static void DrawCube(Material* material, glm::mat4 &model)
	{
		material->use();
		material->BindTextures();
		glBindVertexArray(CommonAssets::instance->cubeVAO);
		Render::SetVertexShaderParams(material, model);
		Render::SetShaderLightParams(material);
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
	static void DrawSphere(Material* material, glm::mat4 &model)
	{
		material->use();
		Render::SetVertexShaderParams(material, model);
		Render::SetShaderLightParams(material);
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
		DrawQuad();
		glUseProgram(0);
	}

private:

    static glm::vec4 clearColor;
	static int screenWidth;
	static int screenHeight;

	static Shader* framebufferDebugShader;
	static bool framebufferDebugInitialized;

};
#endif
