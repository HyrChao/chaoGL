#pragma once
#ifndef MOUSE_H
#define MOUSE_H

class Mouse
{
public:

	static void UpdateMouse(float xpos, float ypos);
	static void SetSensitivity(float sensi);
	static void SetMouseInitLocation(int screemWidth, int screemHeight);

public:
	static float xoffset;
	static float yoffset;

	static void CursorOn(bool ison = true);
	static void CursorToggle();
	static bool GetCursorState();

private:

	static float lastX, lastY;
	static float sensitivity;
	static bool firstMouse;
	static bool cursorToggle;
};


#endif
