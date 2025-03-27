#pragma once
#include "Graphics/Graphics.h"
#include <vector>

#include "Systems/Components.h"

class CGeometry : public CComponent
{
public:
	CGeometry(AActor* InOwner);

	void SetVertices(const std::vector<float>& InVertices);
	void SetIndecies(const std::vector<uint32_t>& InIndecies);
	void SetColors(const std::vector<float>& InColors);

	SGfxVertexDataPtr GetVertexData() const { return VertexData; }
	void BuildGeometry();

private:
	SGfxVertexDataPtr VertexData;

	std::vector<float> Vertices;
	std::vector<float> Colors;
	std::vector<uint32_t> Indecies;

};