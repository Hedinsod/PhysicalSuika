#include "pch.h"
#include "GeometryComp.h"


CGeometry::CGeometry(AActor* InOwner)
	: CComponent(InOwner)
{
}

void CGeometry::Import(const std::vector<glm::vec2>& InVertices)
{
	Indices.reserve(InVertices.size() + 1);
	Vertices.reserve(InVertices.size());
	for (int32_t i = 0; i < InVertices.size(); i++)
	{
		Vertices.emplace_back(InVertices[i].x, InVertices[i].y, 0.0f, 1.0f);
		Indices.emplace_back(i);
	}
	//Indices.emplace_back(0);
}

void CGeometry::SetIndices(const std::vector<uint32_t>& InIndices)
{
	Indices = InIndices;
}

void CGeometry::SetIndices(std::vector<uint32_t>&& InIndices)
{
	Indices = std::move(InIndices);
}
