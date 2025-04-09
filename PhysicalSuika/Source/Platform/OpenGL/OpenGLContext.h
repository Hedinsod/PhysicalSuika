#pragma once

#include "Graphics/GfxContext.h"

struct GLFWwindow;

class SOpenGLContext : public SGfxContext
{
public:
	SOpenGLContext(GLFWwindow* InWindow);
	virtual void SwapBuffers() override;

	// "Events"
	virtual void OnResize(int32_t InWidth, int32_t InHeight) override;

private:
	GLFWwindow* Window;

};