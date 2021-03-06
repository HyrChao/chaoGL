﻿#pragma once
#ifndef RENDER_H
#define RENDER_H

#include <Render/Camera.h>
#include <Render/Light.h>
#include <Render/Capture.h>
#include <Render/Shadow.h>
#include <Render/IDrawable.h>

typedef 

class Render
{
public:

	Render();
	~Render();

	static void PrepareRender();
	static void DrawOnFrameBegin();
	static void Draw();
	static void DrawOnFrameEnd();

	//static void Draw(IDrawable* drawable, Material* material, glm::mat4 modelMat);

	static void AddToCurrentDrawableList(IDrawable * drawable, Material * material, glm::mat4* model);
	static void BindCurrentDrawableList(DrawableList& list);
	static void UnbindCurrentDrawableList();
	static void ClearCurrentDrawableList();


	static void UpdateShaderLightParams();

	static void UpdateShaderCameraVP();

	static void SetMaterialCameraVP(Material* material);

	static void SetVertexShaderParams(Shader *shader, glm::mat4 model = glm::mat4(1.0));

	static void SetClearColor(glm::vec4 color);
	static void WireframeMode(bool on = false);

	// Debug frame buffer texture
	static void DisplayFramebufferTexture(Texture texture);

private:
    
    static void DrawLightAvatars();
	static void ExcuteMainDrawlist();
	static void ExcuteDrawlistWithReplacedMaterial(Material* material);


public:

    static glm::mat4 projectMat;
    static glm::mat4 viewMat;
    static glm::vec3 viewPos;

	static Light* sunlight;




private:


	static DrawableList* currentDrawablelist;

    static glm::vec4 clearColor;
	static Shader* framebufferDebugShader;
	static bool framebufferDebugInitialized;


};
#endif
