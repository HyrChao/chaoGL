#include<Application/Render.h>

Render* Render::render;

Render::Render()
{
	if (render == nullptr)
	{
		render = this;
	}
	else
	{
		delete this;
	}

	SetupRenderProperty();
}

Render::~Render()
{
	delete this;
}

void Render::SetupRenderProperty()
{
	// enable Z test
	glEnable(GL_DEPTH_TEST);
}

void Render::DrawOnFrameBegin()
{
	//// Clear
	//float timeValue = glfwGetTime();
	//glClearColor(0.8f + 0.2*sin(timeValue), 0.8f + 0.2*sin(timeValue + 3.14 / 3), 0.8f + 0.2*sin(timeValue - 3.14 / 3), 1.0f);
	glClearColor(0.0f, 0.0f, 0.0, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Render::DrawOnFrameEnd()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}




