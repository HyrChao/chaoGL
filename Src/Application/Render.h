#pragma once
#ifndef RENDER_H
#define RENDER_H

//#include <GLFW/glfw3.h>
#include<chaoGL.h>


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
