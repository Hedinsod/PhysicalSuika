#include "pch.h"
#include "GfxBuffers.h"
#include "Graphics.h"


// ****************************************************************************
// ********** SGfxBufferLayout ************************************************
// ****************************************************************************

static uint32_t GetShaderDataSize(EGfxShaderData Data)
{
	switch (Data)
	{
	case EGfxShaderData::Float: return 4;
	case EGfxShaderData::Float2: return 4 * 2;
	case EGfxShaderData::Float3: return 4 * 3;
	case EGfxShaderData::Float4: return 4 * 4;
	case EGfxShaderData::Mat3: return 4 * 3 * 3;
	case EGfxShaderData::Mat4: return 4 * 4 * 4;
	case EGfxShaderData::Int: return 4;
	case EGfxShaderData::Int2: return 4 * 2;
	case EGfxShaderData::Int3: return 4 * 3;
	case EGfxShaderData::Int4: return 4 * 4;
	case EGfxShaderData::Bool: return 1;
	}

	GAssert(false);
	return 0;
}

static uint32_t GetShaderDataCount(EGfxShaderData Data)
{
	switch (Data)
	{
	case EGfxShaderData::Float: return 1;
	case EGfxShaderData::Float2: return 2;
	case EGfxShaderData::Float3: return 3;
	case EGfxShaderData::Float4: return 4;
	case EGfxShaderData::Mat3: return 3 * 3;
	case EGfxShaderData::Mat4: return 4 * 4;
	case EGfxShaderData::Int: return 1;
	case EGfxShaderData::Int2: return 2;
	case EGfxShaderData::Int3: return 3;
	case EGfxShaderData::Int4: return 4;
	case EGfxShaderData::Bool: return 1;
	}

	GAssert(false);
	return 0;
}

FGfxBufferElement::FGfxBufferElement(const std::string& InName, EGfxShaderData InType, bool bInNormalized /*= false*/)
	: Name(InName)
	, Type(InType)
	, Size(GetShaderDataSize(InType))
	, Count(GetShaderDataCount(InType))
	, Offset(0)
	, bNormalized(bInNormalized)
{
}

void SGfxBufferLayout::CalculateOffsetAndStride()
{
	Stride = 0;
	for (auto& Element : Elements)
	{
		Element.Offset = Stride;
		Stride += Element.Size;
	}
}

// ****************************************************************************
// ********** SGfxVertexData **************************************************
// ****************************************************************************

std::shared_ptr<SGfxVertexBuffer> SGfxVertexData::AddVertexData(const std::vector<float>& VertexData,
                                                                const SGfxBufferLayout& InLayout)
{
	SGfxVertexBufferPtr NewVertexBuffer = SGraphics::CreateVertexBuffer(VertexData);
	VertexBuffers.push_back(NewVertexBuffer);

	return NewVertexBuffer;
}

void SGfxVertexData::SetIndexData(const std::vector<uint32_t>& IndexData)
{
	IndexBuffer = SGraphics::CreateIndexBuffer(IndexData);
}
