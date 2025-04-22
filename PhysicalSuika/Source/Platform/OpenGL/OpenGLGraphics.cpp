#include "pch.h"
#include "OpenGLGraphics.h"
#include "OpenGLWindow.h"
#include "OpenGLShader.h"
#include "OpenGLBuffers.h"
#include "OpenGLTexture.h"

#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>


SOpenGLGraphics::SOpenGLGraphics()
{
	int Success = glfwInit();
	GAssert(Success);
	glfwSetErrorCallback(ProcessError);
}

SOpenGLGraphics::~SOpenGLGraphics()
{
	glfwTerminate();
}

StdScoped<SGfxWindow> SOpenGLGraphics::CreateGfxWindow(uint32_t InWidth, uint32_t InHeight, const std::string& InTitle)
{
	StdScoped<SGfxWindow> Window = MakeScoped<SOpenGLWindow>(InWidth, InHeight, InTitle);
	PostInit();

    return Window;
}

// Factories
StdScoped<SGfxShaderFactory> SOpenGLGraphics::GetShaderFactory()
{
	return MakeScoped<SOpenGLShaderFactory>();
}
StdScoped<SGfxBufferFactory> SOpenGLGraphics::GetBufferFactory()
{
	return MakeScoped<SOpenGLBufferFactory>();
}
StdScoped<SGfxTextureFactory> SOpenGLGraphics::GetTextureFactory()
{
	return MakeScoped<SOpenGLTextureFactory>();
}

// Render Commands
void SOpenGLGraphics::DrawIndexed(uint32_t IndexCount)
{
	glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, nullptr);
}

void SOpenGLGraphics::SetClearColor(const FColorRGB& InColor)
{
	FColorLinear LinearColor(InColor);
	glClearColor(LinearColor.ColorVec4.r, LinearColor.ColorVec4.g, LinearColor.ColorVec4.b, 1.0f);
}

void SOpenGLGraphics::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


// static callbacks
void SOpenGLGraphics::ProcessError(int32_t ErrorCode, const char* Description)
{
	Log::Log("SOpenGLWindow Error: " + std::string(Description));
}

void SOpenGLGraphics::PostInit()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
}
