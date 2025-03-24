#pragma once
#include "Graphics/Graphics.h"
#include <vector>

class AActor;

struct FVertex
{
	FPoint Vertex;
	FColor Color;
};

struct CGeometry
{
	CGeometry(AActor* InOwner);

	void AddPoint(const FPoint& InPoint, const FColor& InColor);
	void SetPoint(int Index, const FPoint& InPoint);

	std::vector<FVertex> VertexData;

	/*
	std::vector<float> Vertices;
	std::vector<FColor> VertexColor;
	*/

	AActor* Owner;
};