#include <Render/RenderDevice.h>

int RenderDevice::screenHeight;
int RenderDevice::screenWidth;

RenderDevice::RenderDevice()
{
}

RenderDevice::~RenderDevice()
{
}

void RenderDevice::SetScreenSize(int screenWidth, int screenHeight)
{
	RenderDevice::screenHeight = screenHeight;
	RenderDevice::screenWidth = screenWidth;
}

void RenderDevice::SetViewport(int screenWidth, int screenHeight)
{

	glViewport(0, 0, screenWidth, screenHeight);
}

void RenderDevice::ResetViewport()
{
	glViewport(0, 0, screenWidth, screenHeight);
}
