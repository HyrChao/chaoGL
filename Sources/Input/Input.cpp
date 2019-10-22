#include<Input/Input.h>

bool Input::keyOnce[GLFW_KEY_LAST + 1];
bool Input::keyInCold[GLFW_KEY_LAST + 1];

GLFWwindow* Input:: window;


Input::Input()
{

}

void Input::UpdateKeys()
{
	if (window == nullptr)
		return;

	for (int key = 0; key < GLFW_KEY_LAST + 1; key++)
	{
		if (glfwGetKey(window, key) == GLFW_PRESS && !keyInCold[key])
		{
			keyOnce[key] = true;
			keyInCold[key] = true;
		}
		else
			keyOnce[key] = false;

		if (glfwGetKey(window, key) == GLFW_RELEASE)
			keyInCold[key] = false;
	}
}






