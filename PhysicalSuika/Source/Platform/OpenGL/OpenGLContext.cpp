#include "pch.h"
#include "Core/Application.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "Platform/OpenGL/OpenGLWindow.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>


SOpenGLContext::SOpenGLContext(GLFWwindow* InWindow)
	: Window(InWindow)
{
	glfwMakeContextCurrent(Window);

	// Can be called only if context exists
	int32_t Status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	GAssertLog(Status, "Failed to initialize GLAD");

	glfwSwapInterval(1);

	GApp->AddOnResizeEventHandler(std::bind(&SOpenGLContext::OnResize, this, std::placeholders::_1, std::placeholders::_2));
}

void SOpenGLContext::SwapBuffers()
{
	glfwSwapBuffers(Window);
}

void SOpenGLContext::OnResize(int32_t InWidth, int32_t InHeight)
{
	glViewport(0, 0, InWidth, InHeight);
}
