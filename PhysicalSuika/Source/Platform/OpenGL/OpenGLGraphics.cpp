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

std::shared_ptr<SGfxShader> SOpenGLGraphics::CreateShader(const std::string& VertexSource, const std::string& FragmentSource)
{
	return std::make_shared<SOpenGLShader>(VertexSource, FragmentSource);
}

std::shared_ptr<SGfxVertexBuffer> SOpenGLGraphics::CreateVertexBuffer(const std::vector<float>& VertexData)
{
	return std::make_shared<SOpenGLVertexBuffer>(VertexData);
}

std::shared_ptr<SGfxIndexBuffer> SOpenGLGraphics::CreateIndexBuffer(const std::vector<uint32_t>& IndexData)
{
	return std::make_shared<SOpenGLIndexBuffer>(IndexData);
}

std::shared_ptr<SGfxVertexData> SOpenGLGraphics::CreateVertexData()
{
	return std::make_shared<SOpenGLVertexArray>();
}

// Render Commands
void SOpenGLGraphics::DrawIndexed(const std::shared_ptr<SGfxVertexData>& VertexData)
{
	glDrawElements(GL_LINE_STRIP, VertexData->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
}

void SOpenGLGraphics::SetClearColor(const FColorRGB& InColor)
{
	FColorLinear LinearColor(InColor);
	glClearColor(LinearColor.Red, LinearColor.Green, LinearColor.Blue, 1.0f);
}

void SOpenGLGraphics::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}
