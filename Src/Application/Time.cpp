#include<Application/Time.h>

float Time::deltaTime;
float Time::time;
float Time::lastFrame;

void Time::UpdateTime()
{
	lastFrame = time;
	time = glfwGetTime();
	deltaTime = time - lastFrame;
}





