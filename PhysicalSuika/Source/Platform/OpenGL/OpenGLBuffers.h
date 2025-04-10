#pragma once

#include "Graphics/GfxBuffers.h"

class SOpenGLVertexBuffer : public SGfxVertexBuffer
{
public:
	SOpenGLVertexBuffer(const std::vector<float>& VertexData);
	~SOpenGLVertexBuffer();

	virtual void Bind() override;
	virtual void Unbind() override;

	virtual void SetLayout(const SGfxBufferLayout& InLayout) override;

private:
	uint32_t BufferId;

};

class SOpenGLIndexBuffer : public SGfxIndexBuffer
{
public:
	SOpenGLIndexBuffer(const std::vector<uint32_t>& IndexData);
	~SOpenGLIndexBuffer();

	virtual void Bind() override;
	virtual void Unbind() override;

	virtual uint32_t GetCount() override { return Count; }

private:
	uint32_t BufferId;
	uint32_t Count;

};

class SOpenGLVertexArray : public SGfxVertexData
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