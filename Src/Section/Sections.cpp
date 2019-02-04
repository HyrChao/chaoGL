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
		if (defaultSection)
			defaultSection = false;
		else
			defaultSection = true;
	}
	if (glfwGetKey(Application::GetWindow(), GLFW_KEY_ENTER) == GLFW_RELEASE)
		enterKeyPressing = false;

	if (defaultSection)
	{
		hello->HelloBox();
	}
	else
	{
		hello->HelloProjection();
	}
}

