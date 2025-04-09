#pragma once
#include "Graphics/Graphics.h"
#include <vector>

#include "Systems/Components.h"

class CGeometry : public CComponent
{
public:
	CGeometry(AActor* InOwner);

	void SetVertices(const std::vector<float>& InVertices);
	void SetIndices(const std::vector<uint32_t>& InIndices);

	std::shared_ptr<SGfxVertexData> GetVertexData() const { return VertexData; }
	void BuildGeometry();

private:
	std::shared_ptr<SGfxVertexData> VertexData;

	std::vector<float> Vertices;
	std::vector<float> Colors;
	std::vector<uint32_t> Indices;

};