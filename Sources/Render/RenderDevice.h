#ifndef RENDERDEVICE_H
#define RENDERDEVICE_H

#include <glad/glad.h>

class RenderDevice
{
public:
	RenderDevice();
	~RenderDevice();
	
	static void SetViewport(int screenWidth, int screenHeight);
	static void ResetViewport();


	static void BindFrameBuffer(unsigned int FBO);
	static void BindDefaultFrameBuffer();
	static void BindMainFrameBuffer();

	static void UpdateMainFrameBuffer();
	static void InitMainFrameBuffer();

private:
	static unsigned int mainFrameBufferFBO;

public:

};

#endif // !RENDERDEVICE_H
