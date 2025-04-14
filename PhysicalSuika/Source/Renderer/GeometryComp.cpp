#include "pch.h"
#include "GeometryComp.h"


CGeometry::CGeometry(AActor* InOwner)
	: CComponent(InOwner)
	, MaterialTag("Default")
{
}

void CGeometry::Import(const std::vector<glm::vec2>& InVertices)
{
	Indices.reserve(InVertices.size() * 2);
	Vertices.reserve(InVertices.size());
	for (int32_t i = 0; i < InVertices.size(); i++)
	{
		Vertices.emplace_back(InVertices[i].x, InVertices[i].y, 0.0f, 1.0f);
	}
	for (int32_t i = 1; i < InVertices.size(); i++)
	{
		Indices.emplace_back(i-1);
		Indices.emplace_back(i);
	}
}

void CGeometry::SetIndices(const std::vector<uint32_t>& InIndices)
{
	Indices = InIndices;
}

void CGeometry::SetIndices(std::vector<uint32_t>&& InIndices)
{
	Indices = std::move(InIndices);
}
