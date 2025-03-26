#pragma once

#include "Graphics/GfxBuffers.h"
#include <memory>

class SWinApiVertexBuffer : public SGfxVertexBuffer
{
public:
	SWinApiVertexBuffer(const std::vector<float>& InVertexData)
		: VertexData(InVertexData)
	{ }
	virtual ~SWinApiVertexBuffer() = default;

	virtual void Bind() {}
	virtual void Unbind() {}
	virtual void SetLayout() {}

private:
	std::vector<float> VertexData;

};

class SWinApiIndexBuffer : public SGfxIndexBuffer
{
public:
	SWinApiIndexBuffer(const std::vector<uint32_t>& InIndexData)
		: IndexData(InIndexData)
	{ }

	virtual ~SWinApiIndexBuffer() = default;

	virtual void Bind() override {}
	virtual void Unbind() override {}

	virtual uint32_t GetCount() override { return (uint32_t)IndexData.size(); };

private:
	std::vector<uint32_t> IndexData;

};

class SWinApiVertexArray : public SGfxVertexData
{
public:
	SWinApiVertexArray() = default;
	virtual ~SWinApiVertexArray() = default;

	virtual void Bind() {}
	virtual void Unbind() {}

	virtual std::shared_ptr<SGfxVertexBuffer> AddVertexData(const std::vector<float>& VertexData, const SGfxBufferLayout& InLayout) override;
	virtual void SetIndexData(const std::vector<uint32_t>& IndexData) override;

private:

};
