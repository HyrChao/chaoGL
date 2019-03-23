#pragma once
#ifndef RENDER_H
#define RENDER_H

//#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <chaoGL.h>
#include <Render/Shader.h>

class Render
{
public:

	Render();
	~Render();
	static void SetupRenderProperty();
	static void DrawOnFrameBegin();
	static void DrawOnFrameEnd();

public:

	static Render* render;

private:


};
#endif
