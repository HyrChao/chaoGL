#ifndef RENDERDEVICE_H
#define RENDERDEVICE_H

#include <glad/glad.h>

class RenderDevice
{
public:
	RenderDevice();
	~RenderDevice();


	static void SetScreenSize(int screenWidth, int screenHeight)
	{
		RenderDevice::screenHeight = screenHeight;
		RenderDevice::screenWidth = screenWidth;
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

public:

	static int screenWidth;
	static int screenHeight;

};

#endif // !RENDERDEVICE_H
