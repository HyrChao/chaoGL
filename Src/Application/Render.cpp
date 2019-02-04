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
}

Render::~Render()
{
	delete this;
}




