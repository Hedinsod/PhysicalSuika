#include "pch.h"
#include "OpenGLGraphics.h"
#include "OpenGLWindow.h"
#include "OpenGLShader.h"
#include "OpenGLBuffers.h"

#include <glad/glad.h>

StdScoped<SGfxShaderFactory> SOpenGLGraphics::GetShaderFactory()
{
	return MakeScoped<SOpenGLShaderFactory>();
}

// Fabrics
SGfxWindow* SOpenGLGraphics::CreateGfxWindow(int InWidth, int InHeight, const std::string& InTitle)
{
    return new SOpenGLWindow(InWidth, InHeight, InTitle);
}

StdShared<SGfxVertexBuffer> SOpenGLGraphics::CreateVertexBuffer(const std::vector<float>& VertexData)
{
	return MakeShared<SOpenGLVertexBuffer>(VertexData);
}

StdShared<SGfxIndexBuffer> SOpenGLGraphics::CreateIndexBuffer(const std::vector<uint32_t>& IndexData)
{
	return MakeShared<SOpenGLIndexBuffer>(IndexData);
}

StdShared<SGfxVertexData> SOpenGLGraphics::CreateVertexData()
{
	return MakeShared<SOpenGLVertexArray>();
}

// Render Commands
void SOpenGLGraphics::DrawIndexed(const StdShared<SGfxVertexData>& VertexData)
{
	glDrawElements(GL_LINE_STRIP, VertexData->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
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
