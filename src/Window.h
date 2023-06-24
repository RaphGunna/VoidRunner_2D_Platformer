#pragma once

#include "GLFW/glfw3.h"

class Window
{
public:
	static GLFWwindow* window;
	static float Width;
	static float Height;

	static void CreateWindow();
	static void DestroyWindow();

	static void SwapBuffers();
	static void PollEvents();

	static void SetKeyCallback(GLFWkeyfun callback);
};