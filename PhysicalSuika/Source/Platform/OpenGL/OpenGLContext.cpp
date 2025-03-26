#include "pch.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "Platform/OpenGL/OpenGLWindow.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>


SOpenGLContext::SOpenGLContext(GLFWwindow* InWindow)
	: Window(InWindow)
{
	glfwMakeContextCurrent(Window);
	int32_t Status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	GAssert(Status/*, "Failed to initialize GLAD"*/);
}

void SOpenGLContext::SwapBuffers()
{
	glfwSwapBuffers(Window);
}
