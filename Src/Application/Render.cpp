#include<Application/Render.h>

Render* Render::render;

Render::Render()
{
	if (render == nullptr)
	{
		render = this;
		Camera::main = new Camera();
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
void Render::OnFrameEnd()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}




