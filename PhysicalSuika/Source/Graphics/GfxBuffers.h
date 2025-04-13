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
	SGfxVertexBuffer() = default;
	virtual ~SGfxVertexBuffer() = default;

	// Non-copyable
	SGfxVertexBuffer(const SGfxVertexBuffer&) = delete;
	SGfxVertexBuffer& operator=(const SGfxVertexBuffer&) = delete;

	// Upload any data of a given size in bytes
	virtual void UploadVertices(void* Data, size_t Size) = 0;

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
	SGfxIndexBuffer() = default;
	virtual ~SGfxIndexBuffer() = default;

	// Non-copyable
	SGfxIndexBuffer(const SGfxIndexBuffer&) = delete;
	SGfxIndexBuffer& operator=(const SGfxIndexBuffer&) = delete;

	virtual void UploadIndices(uint32_t* Data, size_t Size) = 0;

	virtual void Bind() = 0;
	virtual void Unbind() = 0;

};

// ****************************************************************************
// ********** SGfxBufferFactory ***********************************************
// ****************************************************************************

class SGfxBufferFactory
{
public:
	virtual ~SGfxBufferFactory() = default;

	// VBOs
	virtual StdShared<SGfxVertexBuffer> CreateVertexBuffer(const std::vector<float>& VertexData) = 0;
	virtual StdShared<SGfxVertexBuffer> CreateVertexBuffer(const float* Data, size_t Size) = 0;
	virtual StdShared<SGfxVertexBuffer> CreateVertexBuffer(size_t Size) = 0;

	// IBO(s)
	virtual StdShared<SGfxIndexBuffer> CreateIndexBuffer(const std::vector<uint32_t>& IndexData) = 0;
	virtual StdShared<SGfxIndexBuffer> CreateIndexBuffer(size_t Size) = 0;

	// VAO(s) -- Geometry Object | OpenGL specific?
	/*
	virtual StdShared<SGfxVertexArray> CreateVertexArray(const StdShared<SGfxVertexBuffer>&,
	                                                     const StdShared<SGfxIndexBuffer>&,
	                                                     SGfxBufferLayout&&) = 0;
														 */

};


// ****************************************************************************
// ********** SGfxVertexData **************************************************
// ****************************************************************************
/*
class SGfxVertexArray
{
public:
	virtual ~SGfxVertexArray() = default;

	virtual void Bind() = 0;
	virtual void Unbind() = 0;

	virtual StdShared<SGfxVertexBuffer> AddVertexData(
		const std::vector<float>& VertexData,
		const SGfxBufferLayout& InLayout);

	virtual void SetIndexData(const std::vector<uint32_t>& IndexData);

	uint32_t GetIndexCount() { return IndexBuffer->GetCount(); }

protected:
	std::vector<StdShared<SGfxVertexBuffer>> VertexBuffers;
	StdShared<SGfxIndexBuffer> IndexBuffer;

};
*/
