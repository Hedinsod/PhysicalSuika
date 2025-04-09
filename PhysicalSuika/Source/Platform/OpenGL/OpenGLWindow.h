#pragma once

#include "Graphics/GfxWindow.h"


struct GLFWwindow;

class SOpenGLWindow : public SGfxWindow
{
public:
	SOpenGLWindow(int32_t InWidth, int32_t InHeight, const std::string& InTitle);
	~SOpenGLWindow();

	void Create();
	virtual void Destroy() override;
	virtual void Tick() override;

	virtual void* GetNativeWindow() override { return (void*)NativeWindow; }

	void OnResize(int32_t InWidth, int32_t InHeight);

private:
	static void ProcessError(int32_t ErrorCode, const char* Description);
	static void ProcessInput(GLFWwindow* window, int key, int scancode, int action, int mods);

	GLFWwindow* NativeWindow = nullptr;

	static int32_t Counter;

};
