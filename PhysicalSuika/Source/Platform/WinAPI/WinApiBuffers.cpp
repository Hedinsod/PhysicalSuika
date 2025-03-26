#include "pch.h"
#include "WinApiBuffers.h"

std::shared_ptr<SGfxVertexBuffer> SWinApiVertexArray::AddVertexData(const std::vector<float>& VertexData, const SGfxBufferLayout& InLayout)
{
	return SGfxVertexData::AddVertexData(VertexData, InLayout);
}

void SWinApiVertexArray::SetIndexData(const std::vector<uint32_t>& IndexData)
{
	SGfxVertexData::SetIndexData(IndexData);
}


