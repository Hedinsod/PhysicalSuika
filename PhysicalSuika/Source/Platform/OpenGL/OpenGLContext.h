#pragma once

#include "Graphics/GfxContext.h"

struct GLFWwindow;

class SOpenGLContext : public SGfxContext
{
public:
	SOpenGLContext(GLFWwindow* InWindow);
	virtual void SwapBuffers() override;

private:
	GLFWwindow* Window;

};