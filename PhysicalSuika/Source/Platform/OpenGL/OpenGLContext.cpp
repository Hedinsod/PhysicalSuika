#include "pch.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "Platform/OpenGL/OpenGLWindow.h"

#include <GLFW/glfw3.h>


SOpenGLContext::SOpenGLContext(GLFWwindow* InWindow)
	: Window(InWindow)
{
	glfwMakeContextCurrent(Window);
	//Int32 Status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	//MRAssert(Status, "Failed to initialize GLAD");
}

void SOpenGLContext::SwapBuffers()
{
	glfwSwapBuffers(Window);
}

void SOpenGLContext::FillRect(FPoint Corner1, FPoint Corner2, FColor Color)
{

}

void SOpenGLContext::DrawLine(FPoint Start, FPoint End, FColor color)
{

}