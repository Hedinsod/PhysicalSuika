#include "pch.h"

#include "Core/Application.h"
#include "Core/Input.h"

#include "Platform/OpenGL/OpenGLWindow.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <GLAD/glad.h>

int32_t SOpenGLWindow::Counter;

SOpenGLWindow::SOpenGLWindow(int32_t InWidth, int32_t InHeight, const std::string& InTitle)
	: SGfxWindow(InWidth, InHeight, InTitle)
{
	Create();
}

SOpenGLWindow::~SOpenGLWindow()
{
	Destroy();
}

void SOpenGLWindow::Create()
{
	if (Counter == 0)
	{
		GAssert(glfwInit());
		glfwSetErrorCallback(ProcessError);
	}

	// Actual window
	NativeWindow = glfwCreateWindow(Width, Height, Title.c_str(), NULL, NULL);

	GAssert(NativeWindow);
	GfxContext = new SOpenGLContext(NativeWindow);
	glfwSetWindowUserPointer(NativeWindow, this);

	// Input
	glfwSetKeyCallback(NativeWindow, SOpenGLWindow::ProcessInput);
	glfwSetWindowSizeCallback(NativeWindow, [](GLFWwindow* Window, int InWidth, int InHeight)
		{
			SOpenGLWindow* Self = static_cast<SOpenGLWindow*>(glfwGetWindowUserPointer(Window));
			
			Self->OnResize(InWidth, InHeight);
		});
	glfwSetWindowCloseCallback(NativeWindow, [](GLFWwindow* Window)
		{
			GApp->Quit();
		});
	
	/*
	glfwSetCursorPosCallback(NativeWindow, [](GLFWwindow* Window, double XPos, double YPos)
		{
			SWindowData* Data = static_cast<SWindowData*>(glfwGetWindowUserPointer(Window));
			CMouseMovedEvent Event((float)XPos, (float)YPos);
			Data->EventCallback(Event);
		});
	*/

	glfwSwapInterval(1); // enable VSync
	Counter++;
}

void SOpenGLWindow::Destroy()
{
	glfwDestroyWindow(NativeWindow);

	delete GfxContext;
	GfxContext = nullptr;

	Counter--;
	if (Counter == 0)
	{
		glfwTerminate();
	}
}

void SOpenGLWindow::Tick()
{
	glfwPollEvents();
	GfxContext->SwapBuffers();
}

void SOpenGLWindow::OnResize(int32_t InWidth, int32_t InHeight)
{
	Width = InWidth;
	Height = InHeight;

	GApp->OnResize(Width, Height);
}

// static callbacks
void SOpenGLWindow::ProcessError(int32_t ErrorCode, const char* Description)
{
	Utility::Log("SOpenGLWindow Error");
	Utility::Log(Description);
}

static EInputCode GetInputCode(int32_t GlfwCode)
{
	switch (GlfwCode)
	{
	case GLFW_KEY_LEFT:   return EInputCode::Left;  break;
	case GLFW_KEY_UP:     return EInputCode::Up;    break;
	case GLFW_KEY_RIGHT:  return EInputCode::Right; break;
	case GLFW_KEY_DOWN:   return EInputCode::Down;  break;
	case GLFW_KEY_SPACE:  return EInputCode::Space; break;
	case GLFW_KEY_ESCAPE: return EInputCode::Esc;   break;

	default:              return EInputCode::None;  break;
	};
}

void SOpenGLWindow::ProcessInput(GLFWwindow* Window, int Key, int ScanCode, int Action, int Mods)
{
	switch (Action)
	{
	case GLFW_PRESS:   SInput::InputEvent(GetInputCode(Key), true);  break;
	case GLFW_RELEASE: SInput::InputEvent(GetInputCode(Key), false); break;
	}
}
