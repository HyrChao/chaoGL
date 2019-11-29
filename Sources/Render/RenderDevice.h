#ifndef RENDERDEVICE_H
#define RENDERDEVICE_H

#include <glad/glad.h>

class RenderDevice
{
public:
	RenderDevice();
	~RenderDevice();


	static void SetScreenSize(int screenWidth, int screenHeight);
	
	static void SetViewport(int screenWidth, int screenHeight);
	static void ResetViewport();


private:

public:

	static int screenWidth;
	static int screenHeight;

};

#endif // !RENDERDEVICE_H
