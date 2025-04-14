#include "pch.h"

#include "Core/Application.h"
#include "Core/Input.h"

#include "Platform/OpenGL/OpenGLWindow.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <GLAD/glad.h>


SOpenGLWindow::SOpenGLWindow(int32_t InWidth, int32_t InHeight, const std::string& InTitle)
	: SGfxWindow(InWidth, InHeight, InTitle)
{
	Create();

	CreateContext();
}

SOpenGLWindow::~SOpenGLWindow()
{
	if (NativeWindow)
	{
		Destroy();
	}
}

void SOpenGLWindow::Create()
{
	// Setting up window
	NativeWindow = glfwCreateWindow(Width, Height, Title.c_str(), NULL, NULL);
	GAssert(NativeWindow);

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
}

void SOpenGLWindow::CreateContext()
{
	GfxContext = MakeScoped<SOpenGLContext>(NativeWindow);
}

void SOpenGLWindow::DeleteContext()
{
	GfxContext.reset(nullptr);
}

void SOpenGLWindow::Destroy()
{
	glfwDestroyWindow(NativeWindow);
	NativeWindow = nullptr;

	DeleteContext();
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
