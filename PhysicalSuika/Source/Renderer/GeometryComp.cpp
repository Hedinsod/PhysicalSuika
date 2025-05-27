#include "pch.h"
#include "GeometryComp.h"
#include "Game/Actor.h"


void CGeometry::SetVertices(const std::vector<glm::vec2>& InVertices)
{
	Vertices.reserve(InVertices.size());
	UVs.reserve(InVertices.size());
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

void CGeometry::SetUVs(const std::vector<glm::vec2>& InUVs)
{
	GAssert(InUVs.size() == Vertices.size());

	UVs = InUVs;
}

void CGeometry::SetUVs(std::vector<glm::vec2>&& InUVs)
{
	GAssert(InUVs.size() == Vertices.size());

	UVs = std::move(InUVs);
}

void CGeometry::SetColor(FColorRGB Color)
{
	OverrideColor = Color;
}

void CGeometry::SetMaterial(const std::string& InTag)
{
	MaterialTag = InTag;
}

bool CGeometry::Verify()
{
	bool bResult = true;

	bResult &= UVs.size() == Vertices.size();
	bResult &= Indices.size() >= Vertices.size();

    return bResult;
}
