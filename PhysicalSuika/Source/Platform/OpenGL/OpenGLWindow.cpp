#include "pch.h"

#include "Core/Application.h"
#include "Core/Input.h"
#include "Graphics/Graphics.h"
#include "Platform/OpenGL/OpenGLWindow.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>


SOpenGLWindow::SOpenGLWindow(int32_t InWidth, int32_t InHeight, const std::string& InTitle)
	: SGfxWindow(InWidth, InHeight, InTitle)
{
	Create();
}

void SOpenGLWindow::Create()
{
	if (!bInit)
	{
		GAssert(glfwInit());
		glfwSetErrorCallback(ProcessError);

		bInit = true;
	}

	// Actual window
	NativeWindow = glfwCreateWindow(Width, Height, Title.c_str(), NULL, NULL);
	GAssert(NativeWindow);
	GfxContext = new SOpenGLContext(NativeWindow);

	// Input
	glfwSetKeyCallback(NativeWindow, SOpenGLWindow::ProcessInput);
	//glfwSetWindowSizeCallback(NativeWindow, window_size_callback);
	/*
	glfwSetWindowCloseCallback(NativeWindow, [](GLFWwindow* Window)
		{
			SWindowData* Data = static_cast<SWindowData*>(glfwGetWindowUserPointer(Window));

			CWindowCloseEvent CloseEvent;
			Data->EventCallback(CloseEvent);
		});
	*/

	/*
	glfwSetCursorPosCallback(NativeWindow, [](GLFWwindow* Window, double XPos, double YPos)
		{
			SWindowData* Data = static_cast<SWindowData*>(glfwGetWindowUserPointer(Window));
			CMouseMovedEvent Event((float)XPos, (float)YPos);
			Data->EventCallback(Event);
		});
	*/

	glfwSwapInterval(1); // enable VSync
}

void SOpenGLWindow::Destroy()
{
	glfwDestroyWindow(NativeWindow);

	delete GfxContext;
	GfxContext = nullptr;

	// done once!
	//glfwTerminate();
}

void SOpenGLWindow::Tick()
{
	glfwPollEvents();
	GfxContext->SwapBuffers();
}


// static callbacks
void SOpenGLWindow::ProcessError(int32_t ErrorCode, const char* Description)
{
	std::cerr << "Error:" << Description << std::endl;
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
