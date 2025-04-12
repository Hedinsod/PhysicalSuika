#include "pch.h"
#include "OpenGLGraphics.h"
#include "OpenGLWindow.h"
#include "OpenGLShader.h"
#include "OpenGLBuffers.h"

#include <glad/glad.h>

// Fabrics
SGfxWindow* SOpenGLGraphics::CreateGfxWindow(int InWidth, int InHeight, const std::string& InTitle)
{
    return new SOpenGLWindow(InWidth, InHeight, InTitle);
}

StdScoped<SGfxShaderFactory> SOpenGLGraphics::GetShaderFactory()
{
	return MakeScoped<SOpenGLShaderFactory>();
}
StdScoped<SGfxBufferFactory> SOpenGLGraphics::GetBufferFactory()
{
	return MakeScoped<SOpenGLBufferFactory>();
}



// Render Commands
void SOpenGLGraphics::DrawIndexed(uint32_t IndexCount)
{
	glDrawElements(GL_LINE_STRIP, IndexCount, GL_UNSIGNED_INT, nullptr);
}

void SOpenGLGraphics::SetClearColor(const FColorRGB& InColor)
{
	FColorLinear LinearColor(InColor);
	glClearColor(LinearColor.ColorVec4.r, LinearColor.ColorVec4.g, LinearColor.ColorVec4.b, 1.0f);
}

void SOpenGLGraphics::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}
