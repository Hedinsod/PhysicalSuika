#pragma once
#include "Graphics/Graphics.h"
#include <vector>

class AActor;

class CComponent
{
public:
	CComponent(AActor* InOwner)
		: Owner(InOwner)
	{
	}

private:
	AActor* Owner;

};

class CGeometry : public CComponent
{
public:
	CGeometry(AActor* InOwner);

	void SetVertices(const std::vector<float> InVertices);
	void SetIndecies(const std::vector<uint32_t> InIndecies);

	SGfxVertexDataPtr GetVertexData() const { return VertexData; }
	void BuildGeometry();

private:
	SGfxVertexDataPtr VertexData;

	std::vector<float> Vertices;
	std::vector<float> Colors;
	std::vector<uint32_t> Indecies;

};