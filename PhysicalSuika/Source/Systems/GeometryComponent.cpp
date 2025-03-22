#include "pch.h"
#include "GeometryComponent.h"


void CGeometry::AddPoint(const FPoint& InPoint, const FColor& InColor)
{
	VertexData.push_back({ InPoint, InColor });
}

void CGeometry::SetPoint(int Index, const FPoint& InPoint)
{
	GAssert(Index > 0 && VertexData.size() > Index);

	VertexData[Index].Vertex = InPoint;
}

CGeometry::CGeometry(Entity* InOwner)
	: Owner(InOwner)
{
}
