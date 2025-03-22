#pragma once
#include "Graphics/Types.h"
#include <vector>

class Entity;

struct FVertex
{
	FPoint Vertex;
	FColor Color;
};

struct CGeometry
{
	CGeometry(Entity* InOwner);

	void AddPoint(const FPoint& InPoint, const FColor& InColor);
	void SetPoint(int Index, const FPoint& InPoint);

	std::vector<FVertex> VertexData;

	Entity* Owner;
};