#pragma once

#include <vector>


enum class EGfxShaderData
{
	None = 0,
	Float,
	Float2,
	Float3,
	Float4,
	Mat3,
	Mat4,
	Int,
	Int2,
	Int3,
	Int4,
	Bool,
};

struct FGfxBufferElement
{
	FGfxBufferElement(const std::string& InName, EGfxShaderData InType, bool bInNormalized = false);

	std::string Name;
	uint32_t Size;
	uint64_t Offset;
	uint32_t Count;
	bool bNormalized;

	EGfxShaderData Type;
};

class SGfxBufferLayout
{
public:
	SGfxBufferLayout() = default;
	SGfxBufferLayout(const std::initializer_list<FGfxBufferElement>& InElements)
		: Elements(InElements)
	{
		CalculateOffsetAndStride();
	}

	uint32_t GetStride() const { return Stride; }

	std::vector<FGfxBufferElement>::iterator begin() { return Elements.begin(); }
	std::vector<FGfxBufferElement>::iterator end() { return Elements.end(); }
	std::vector<FGfxBufferElement>::const_iterator begin() const { return Elements.begin(); }
	std::vector<FGfxBufferElement>::const_iterator end() const { return Elements.end(); }

private:
	void CalculateOffsetAndStride();

	std::vector<FGfxBufferElement> Elements;
	uint32_t Stride = 0;
};

// ****************************************************************************
// ********** SGfxVertexBuffer ************************************************
// ****************************************************************************

class SGfxVertexBuffer
{
public:
	virtual ~SGfxVertexBuffer() = default;

	virtual void Bind() = 0;
	virtual void Unbind() = 0;

	virtual void SetLayout(const SGfxBufferLayout& InLayout) = 0;

};

// ****************************************************************************
// ********** SGfxIndexBuffer *************************************************
// ****************************************************************************

class SGfxIndexBuffer
{
public:
	virtual ~SGfxIndexBuffer() = default;

	virtual void Bind() = 0;
	virtual void Unbind() = 0;

	virtual uint32_t GetCount() = 0;

};

// ****************************************************************************
// ********** SGfxVertexData **************************************************
// ****************************************************************************

class SGfxVertexData
{
public:
	virtual ~SGfxVertexData() = default;

	virtual void Bind() = 0;
	virtual void Unbind() = 0;

	virtual std::shared_ptr<SGfxVertexBuffer> AddVertexData(
		const std::vector<float>& VertexData, 
		const SGfxBufferLayout& InLayout);

	virtual void SetIndexData(const std::vector<uint32_t>& IndexData);

	uint32_t GetIndexCount() { return IndexBuffer->GetCount(); }

protected:
	std::vector<std::shared_ptr<SGfxVertexBuffer>> VertexBuffers;
	std::shared_ptr<SGfxIndexBuffer> IndexBuffer;

};