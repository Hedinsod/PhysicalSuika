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
		UVs.emplace_back(0.0f, 0.0f);
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

void CGeometry::SetUVs(std::vector<glm::vec2>&& InUVs)
{
	GAssert(InUVs.size() == Vertices.size());

	UVs = std::move(InUVs);
}
