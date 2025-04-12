#pragma once

#include "Graphics/GfxBuffers.h"


// ****************************************************************************
// ********** SOpenGLVertexBuffer *********************************************
// ****************************************************************************

class SOpenGLVertexBuffer : public SGfxVertexBuffer
{
public:
	SOpenGLVertexBuffer(uint32_t InBufferId) : BufferId(InBufferId) { }
	virtual ~SOpenGLVertexBuffer();

	virtual void UploadVertices(void* Data, size_t Size) override;

	virtual void Bind() override;
	virtual void Unbind() override;

	virtual void SetLayout(const SGfxBufferLayout& InLayout) override;

private:
	uint32_t BufferId;

};

// ****************************************************************************
// ********** SOpenGLIndexBuffer **********************************************
// ****************************************************************************

class SOpenGLIndexBuffer : public SGfxIndexBuffer
{
public:
	SOpenGLIndexBuffer(uint32_t InBufferId) : BufferId(InBufferId) {}
	virtual ~SOpenGLIndexBuffer();

	virtual void UploadIndices(uint32_t* Data, size_t Size) override;

	virtual void Bind() override;
	virtual void Unbind() override;

	//virtual uint32_t GetCount() override { return Count; }

private:
	uint32_t BufferId;
	//uint32_t Count;

};

// ****************************************************************************
// ********** SGfxBufferFactory ***********************************************
// ****************************************************************************

class SOpenGLBufferFactory : public SGfxBufferFactory
{
public:
	virtual ~SOpenGLBufferFactory();

	// Create vertex buffer and upload values from a given vector
	virtual StdShared<SGfxVertexBuffer> CreateVertexBuffer(const std::vector<float>& VertexData) override;
	// Create vertex buffer and upload values from a given C-array of Count elements
	virtual StdShared<SGfxVertexBuffer> CreateVertexBuffer(const float* Data, size_t Count) override;
	// Create empty vertex buffer of a give size in bytes
	virtual StdShared<SGfxVertexBuffer> CreateVertexBuffer(size_t Size) override;

	// Create an index buffer and upload values from a given vector
	virtual StdShared<SGfxIndexBuffer> CreateIndexBuffer(const std::vector<uint32_t>& IndexData) override;

	virtual StdShared<SGfxIndexBuffer> CreateIndexBuffer(size_t Size) override;

	//	virtual void CreateVertexArray(/*VertexBuffer, IndexBuffer, Layout*/) override;    // Geometry Object

};


/*
class SOpenGLVertexArray : public SGfxVertexArray
{
public:
	SOpenGLVertexArray();
	virtual ~SOpenGLVertexArray();

	virtual void Bind() override;
	virtual void Unbind() override;

	virtual StdShared<SGfxVertexBuffer> AddVertexData(const std::vector<float>& VertexData,
															const SGfxBufferLayout& InLayout) override;
	virtual void SetIndexData(const std::vector<uint32_t>& IndexData) override;

private:
	uint32_t ArrayId;

};
*/