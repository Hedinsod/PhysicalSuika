#pragma once

#include <vector>
#include "Systems/Components.h"

class CGeometry : public CComponent
{
	friend class SGeometryPool;
public:
	CGeometry(AActor* InOwner);

	void Import(const std::vector<glm::vec2>& InVertices);

	void SetIndices(const std::vector<uint32_t>& InIndices);
	void SetIndices(std::vector<uint32_t>&& InIndices);


private:
	std::vector<glm::vec4> Vertices;
	std::vector<uint32_t> Indices;

};