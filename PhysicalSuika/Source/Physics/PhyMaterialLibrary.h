#pragma once

#include <vector>


using FPhyMaterialHandle = uint32_t;

struct FPhysicalMaterial
{
	float Density = 0.0f;
	float Friction = 0.0f;

	float GravityScale = 0.0f;
};

class SPhyMatirialLibrary
{
public:
	static uint32_t AddMaterial(FPhysicalMaterial NewMat)
	{
		uint32_t Id = (uint32_t)Library.size();
		Library.push_back(NewMat);

		return Id;
	}

	static const FPhysicalMaterial& GetMaterial(uint32_t Id)
	{
		return Library[Id];    // Relying on std::vector checks
	}

private:
	static std::vector<FPhysicalMaterial> Library;

};
