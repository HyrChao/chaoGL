#pragma once
#ifndef RENDER_H
#define RENDER_H

#include<Application/Camera.h>
class Render
{
public:
	Render();
	~Render();
	static void SetupRenderProperty();
	static void OnFrameEnd();

public:
	static Render* render;

private:


};
#endif
