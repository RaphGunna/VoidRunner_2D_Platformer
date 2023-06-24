#include "Window.h"

GLFWwindow* Window::window;
float Window::Width;
float Window::Height;

void Window::CreateWindow()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	window = glfwCreateWindow(mode->width, mode->height, "", NULL, NULL);
	if (!window) {
		glfwTerminate();
		// handle
	}

	Window::Width = mode->width;
	Window::Height = mode->height;

	glfwMakeContextCurrent(window);

	glfwSwapInterval(0);
}

void Window::DestroyWindow()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(window);
}

void Window::PollEvents()
{
	glfwPollEvents();
}

void Window::SetKeyCallback(GLFWkeyfun callback)
{
	glfwSetKeyCallback(window, callback);
}