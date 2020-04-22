#include <Render/RenderDevice.h>
#include <Application/Application.h>

RenderDevice::RenderDevice()
{
}

RenderDevice::~RenderDevice()
{
}


void RenderDevice::SetViewport(int screenWidth, int screenHeight)
{

	glViewport(0, 0, screenWidth, screenHeight);
}

void RenderDevice::ResetViewport()
{
	glViewport(0, 0, Application::currentWindow->GetWidth(), Application::currentWindow->GetHeight());
}
