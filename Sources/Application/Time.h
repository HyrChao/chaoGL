#pragma once
#ifndef TIME_H
#define TIME_H
#include <GLFW/glfw3.h>

class Time
{
public:
	static void UpdateTime();

public:
	static float deltaTime;
	static float time;
	static float lastFrame;

};
#endif
