#pragma once

#include <vector>
#include "Systems/Components.h"

class CGeometry : public CComponent
{
	friend class SRenderer;
public:
	CGeometry(AActor* InOwner);

	void Import(const std::vector<glm::vec2>& InVertices);
	void SetUVs(std::vector<glm::vec2>&& InUVs);

	void SetMaterial(const std::string InTag) { MaterialTag = InTag; }

	void SetIndices(const std::vector<uint32_t>& InIndices);
	void SetIndices(std::vector<uint32_t>&& InIndices);

private:
	std::vector<glm::vec4> Vertices;
	std::vector<glm::vec2> UVs;

	std::vector<uint32_t> Indices;

	// Weak pointer
	std::string MaterialTag;

};