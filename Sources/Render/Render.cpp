#include<Render/Render.h>

Render* Render::render;
glm::vec4 Render::clearColor;
glm::mat4 Render::projectMat;
glm::mat4 Render::viewMat;
glm::vec3 Render::viewPos;

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
	glClearColor(clearColor.x,clearColor.y,clearColor.z, clearColor.w);
	glClear(GL_COLOR_BUFFER_BIT);
    
    // Update camera
    Camera::main->UpdateCamera();
    
    // move model forward equals to move view backward
    viewMat = Camera::main->viewMat;
    projectMat = Camera::main->projMat;
    viewPos = Camera::main->pos;
}

void Render::DrawOnFrameEnd()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}




