#pragma once
#ifndef RENDER_H
#define RENDER_H

//#include <GLFW/glfw3.h>
#include <glad/glad.h>
//#include <Render/Shader.h>
#include <Render/Texture.h>
#include <Render/Camera.h>
#include <Render/Light.h>

class Render
{
public:

	Render();
	~Render();
	static void SetupRenderProperty();
	static void DrawOnFrameBegin();
	static void DrawOnFrameEnd();
    static void SetShaderParams(Shader *shader, glm::mat4 model = glm::mat4(1.0))
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
    
private:

    static glm::vec4 clearColor;

};
#endif
