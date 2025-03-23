#pragma once

#include "Graphics/GfxContext.h"

struct GLFWwindow;

class SOpenGLContext : public SGfxContext
{
public:
	SOpenGLContext(GLFWwindow* InWindow);
	virtual void SwapBuffers() override;

	virtual void FillRect(FPoint Pivot, FPoint Size, FColor Color) override;
	virtual void DrawLine(FPoint Start, FPoint End, FColor color) override;

private:
	GLFWwindow* Window;

};