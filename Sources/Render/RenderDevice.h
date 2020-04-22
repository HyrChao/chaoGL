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


private:

public:

};

#endif // !RENDERDEVICE_H
