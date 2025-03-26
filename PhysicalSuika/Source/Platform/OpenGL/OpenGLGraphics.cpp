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

SGfxShaderPtr SOpenGLGraphics::CreateShader(const std::string& VertexSource, const std::string& FragmentSource)
{
	return std::make_shared<SOpenGLShader>(VertexSource, FragmentSource);
}

SGfxVertexBufferPtr SOpenGLGraphics::CreateVertexBuffer(const std::vector<float>& VertexData)
{
	return std::make_shared<SOpenGLVertexBuffer>(VertexData);
}

SGfxIndexBufferPtr SOpenGLGraphics::CreateIndexBuffer(const std::vector<uint32_t>& IndexData)
{
	return std::make_shared<SOpenGLIndexBuffer>(IndexData);
}

SGfxVertexDataPtr SOpenGLGraphics::CreateVertexData()
{
	return std::make_shared<SOpenGLVertexArray>();
}

// Render Commands
void SOpenGLGraphics::DrawIndexed(const SGfxVertexDataPtr& VA)
{
	glDrawElements(GL_LINE_LOOP, VA->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
}

void SOpenGLGraphics::SetClearColor(const FColor& InColor)
{
	glClearColor(InColor.Red/255.f, InColor.Green/255.f, InColor.Blue/255.f, 1.0f);
}

void SOpenGLGraphics::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}
