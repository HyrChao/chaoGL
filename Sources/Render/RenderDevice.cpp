#include <Render/RenderDevice.h>
#include <Application/Application.h>

unsigned int RenderDevice::mainFrameBufferFBO;

RenderDevice::RenderDevice()
{
}

RenderDevice::~RenderDevice()
{
}

void RenderDevice::BindDefaultFrameBuffer()
{
	// back to default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
}

void RenderDevice::BindMainFrameBuffer()
{
	// Use generated buffer as framebuffer's texture buffer
	glBindFramebuffer(GL_FRAMEBUFFER, mainFrameBufferFBO);
}


void RenderDevice::SetViewport(int screenWidth, int screenHeight)
{

	glViewport(0, 0, screenWidth, screenHeight);
}

void RenderDevice::ResetViewport()
{
	glViewport(0, 0, Application::currentWindow->GetWidth(), Application::currentWindow->GetHeight());
}

void RenderDevice::UpdateMainFrameBuffer()
{
	static bool bufferRefresh = false;

	static unsigned int prevWidth;
	static unsigned int prevHeight;

	unsigned int resWidth = (unsigned int)Application::currentWindow->GetWidth();
	unsigned int resHeight = (unsigned int)Application::currentWindow->GetHeight();

	if (prevHeight != resHeight || prevWidth != resWidth)
	{
		prevWidth = resWidth;
		prevHeight = resHeight;

		bufferRefresh = true;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, mainFrameBufferFBO);

	if (bufferRefresh)
	{
		// resize texture
		glBindTexture(GL_TEXTURE_2D, CommonAssets::instance->backBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, resWidth, resHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		glBindTexture(GL_TEXTURE_2D, CommonAssets::instance->depthBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, resWidth, resHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

		// attach texture to framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, CommonAssets::instance->backBuffer, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, CommonAssets::instance->depthBuffer, 0);

		//glActiveTexture(0);
		bufferRefresh = false;
	}

	// Clear depth with white color (infinite far)
	glClearColor(2.0, 2.0, 2.0, 0.0);
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	//glDisable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void RenderDevice::InitMainFrameBuffer()
{
	static bool bufferGenerated = false;
	if (!bufferGenerated)
	{
		// create frame buffer
		glGenFramebuffers(1, &mainFrameBufferFBO);

		// back buffer texture
		glGenTextures(1, &CommonAssets::instance->backBuffer);
		glBindTexture(GL_TEXTURE_2D, CommonAssets::instance->backBuffer);
		// set back buffer texture format
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 800, 800, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		// depth buffer texture
		glGenTextures(1, &CommonAssets::instance->depthBuffer);
		glBindTexture(GL_TEXTURE_2D, CommonAssets::instance->depthBuffer);
		// set depth buffer texture format
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, 800, 800, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		// attach texture to framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, CommonAssets::instance->backBuffer, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, CommonAssets::instance->depthBuffer, 0);
		glActiveTexture(0);

		// check frame buffer state
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Generate framebuffer complete" << std::endl;

		// unbind buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		bufferGenerated = true;
	}
}
