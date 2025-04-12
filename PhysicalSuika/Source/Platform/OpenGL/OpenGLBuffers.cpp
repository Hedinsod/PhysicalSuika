#include "pch.h"
#include "OpenGLBuffers.h"

#include <glad/glad.h>


// ****************************************************************************
// ********** SOpenGLVertexBuffer *********************************************
// ****************************************************************************

SOpenGLVertexBuffer::~SOpenGLVertexBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &BufferId);
}

void SOpenGLVertexBuffer::UploadVertices(void* Data, size_t Size)
{
	glBindBuffer(GL_ARRAY_BUFFER, BufferId); // ?
	glBufferData(GL_ARRAY_BUFFER, Size, Data, GL_DYNAMIC_DRAW);
}

void SOpenGLVertexBuffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, BufferId);
}

void SOpenGLVertexBuffer::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

static GLenum GetGLElementType(EGfxShaderData Data)
{
	switch (Data)
	{
	case EGfxShaderData::Float:
	case EGfxShaderData::Float2:
	case EGfxShaderData::Float3:
	case EGfxShaderData::Float4:
	case EGfxShaderData::Mat3:
	case EGfxShaderData::Mat4:
		return GL_FLOAT;
	case EGfxShaderData::Int:
	case EGfxShaderData::Int2:
	case EGfxShaderData::Int3:
	case EGfxShaderData::Int4:
		return GL_INT;
	case EGfxShaderData::Bool:
		return GL_BOOL;
	}

	GAssert(false);
	return 0;
}

void SOpenGLVertexBuffer::SetLayout(const SGfxBufferLayout& InLayout)
{
	glBindBuffer(GL_ARRAY_BUFFER, BufferId);

	int i = 0;
	for (const auto& Element : InLayout)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(
			i++,
			Element.Count,
			GetGLElementType(Element.Type),
			Element.bNormalized ? GL_TRUE : GL_FALSE,
			InLayout.GetStride(),
			reinterpret_cast<void*>(Element.Offset));
	}

	// Unbind
}

// ****************************************************************************
// ********** SOpenGLIndexBuffer **********************************************
// ****************************************************************************

SOpenGLIndexBuffer::~SOpenGLIndexBuffer()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &BufferId);
}

void SOpenGLIndexBuffer::UploadIndices(uint32_t* Data, size_t Size)
{
	// ? ? ?
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Size, Data, GL_DYNAMIC_DRAW);
}

void SOpenGLIndexBuffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferId);
}

void SOpenGLIndexBuffer::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// ****************************************************************************
// ********** SGfxBufferFactory ***********************************************
// ****************************************************************************

SOpenGLBufferFactory::~SOpenGLBufferFactory() = default;

StdShared<SGfxVertexBuffer> SOpenGLBufferFactory::CreateVertexBuffer(const std::vector<float>& VertexData)
{
	uint32_t RenderId;
	glGenBuffers(1, &RenderId);
	glBindBuffer(GL_ARRAY_BUFFER, RenderId);
	glBufferData(GL_ARRAY_BUFFER, VertexData.size() * sizeof(float), VertexData.data(), GL_STATIC_DRAW);

	return MakeShared<SOpenGLVertexBuffer>(RenderId);
}

StdShared<SGfxVertexBuffer> SOpenGLBufferFactory::CreateVertexBuffer(const float* Data, size_t Count)
{
	uint32_t RenderId;
	glGenBuffers(1, &RenderId);
	glBindBuffer(GL_ARRAY_BUFFER, RenderId);
	glBufferData(GL_ARRAY_BUFFER, Count * sizeof(float), Data, GL_STATIC_DRAW);

	return MakeShared<SOpenGLVertexBuffer>(RenderId);
}

StdShared<SGfxVertexBuffer> SOpenGLBufferFactory::CreateVertexBuffer(size_t Size)
{
	uint32_t RenderId;
	glGenBuffers(1, &RenderId);
	glBindBuffer(GL_ARRAY_BUFFER, RenderId);
	glBufferData(GL_ARRAY_BUFFER, Size, nullptr, GL_STATIC_DRAW);

	return MakeShared<SOpenGLVertexBuffer>(RenderId);
}

StdShared<SGfxIndexBuffer> SOpenGLBufferFactory::CreateIndexBuffer(const std::vector<uint32_t>& IndexData)
{
	uint32_t RenderId;
	glGenBuffers(1, &RenderId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RenderId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexData.size() * sizeof(uint32_t), IndexData.data(), GL_STATIC_DRAW);

	return MakeShared<SOpenGLIndexBuffer>(RenderId);
}

StdShared<SGfxIndexBuffer> SOpenGLBufferFactory::CreateIndexBuffer(size_t Size)
{
	uint32_t RenderId;
	glGenBuffers(1, &RenderId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RenderId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Size, nullptr, GL_STATIC_DRAW);

	return MakeShared<SOpenGLIndexBuffer>(RenderId);
}


// ****************************************************************************
// ********** SOpenGLVertexArray **********************************************
// ****************************************************************************

/*
SOpenGLVertexArray::SOpenGLVertexArray()
{
	glGenVertexArrays(1, &ArrayId);
}
SOpenGLVertexArray::~SOpenGLVertexArray()
{
	glBindVertexArray(0);
}

void SOpenGLVertexArray::Bind()
{
	glBindVertexArray(ArrayId);
}

void SOpenGLVertexArray::Unbind()
{
	glBindVertexArray(0);
}

StdShared<SGfxVertexBuffer> SOpenGLVertexArray::AddVertexData(const std::vector<float>& VertexData,
													  const SGfxBufferLayout& InLayout)
{
	StdShared<SGfxVertexBuffer> NewVertexBuffer = SGfxVertexData::AddVertexData(VertexData, InLayout);

	glBindVertexArray(ArrayId);
	NewVertexBuffer->Bind();
	NewVertexBuffer->SetLayout(InLayout);

	return NewVertexBuffer;
}

void SOpenGLVertexArray::SetIndexData(const std::vector<uint32_t>& IndexData)
{
	SGfxVertexData::SetIndexData(IndexData);

	glBindVertexArray(ArrayId);
	IndexBuffer->Bind();
}
*/