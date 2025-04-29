#pragma once

#include "Systems/Components.h"
#include <vector>


class AActor;
class CTransform;

class CGeometry : public CComponent
{
	friend class SRenderer;
public:
	CGeometry(AActor* InOwner)
		: CComponent(InOwner)
		, MaterialTag("Default")
	{ }

	void SetVertices(const std::vector<glm::vec2>& InVertices);
	void SetIndices(const std::vector<uint32_t>& InIndices);
	void SetIndices(std::vector<uint32_t>&& InIndices);
	void SetUVs(const std::vector<glm::vec2>& InUVs);
	void SetUVs(std::vector<glm::vec2>&& InUVs);

	void SetMaterial(const std::string InTag) { MaterialTag = InTag; }

	bool Verify();

private:
	std::vector<glm::vec4> Vertices;
	std::vector<glm::vec2> UVs;

	std::vector<uint32_t> Indices;

	// Weak pointer
	std::string MaterialTag;

};
