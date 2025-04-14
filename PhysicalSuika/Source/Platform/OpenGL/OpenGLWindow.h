#pragma once

#include "Graphics/GfxWindow.h"


struct GLFWwindow;

class SOpenGLWindow : public SGfxWindow
{
public:
	SOpenGLWindow(int32_t InWidth, int32_t InHeight, const std::string& InTitle);
	~SOpenGLWindow();

	virtual void Destroy() override;
	virtual void Tick() override;

private:
	// Internal events - called by GLFW
	void OnResize(int32_t InWidth, int32_t InHeight);
	static void ProcessInput(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
	void Create();
	void CreateContext();
	void DeleteContext();

	GLFWwindow* NativeWindow = nullptr;

};
