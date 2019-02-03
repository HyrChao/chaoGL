#include <Section\Sections.h>

Sections::Sections()
{
	hello = new Hello();
}

Sections::~Sections()
{
	delete hello;
}

void Sections::SwitchSections()
{
	if (glfwGetKey(Application::GetWindow(), GLFW_KEY_ENTER) == GLFW_PRESS && !enterKeyPressing)
	{
		enterKeyPressing = true;
		if (drawTriangleMode)
			drawTriangleMode = false;
		else
			drawTriangleMode = true;
	}
	if (glfwGetKey(Application::GetWindow(), GLFW_KEY_ENTER) == GLFW_RELEASE)
		enterKeyPressing = false;

	if (drawTriangleMode)
	{
		hello->HelloTriangle();
	}
	else
	{
		hello->HelloTransform();
	}
}

