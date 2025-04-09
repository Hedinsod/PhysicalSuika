#pragma once

#include <unordered_map>
#include <string>
#include <memory>


struct FMaterial
{
	FMaterial() = default;
	FMaterial(float InDensity, float InFriction, float InGravityScale)
		: Density(InDensity)
		, Friction(InFriction)
		, GravityScale(InGravityScale)
	{ }

	float Density = 0.0f;
	float Friction = 0.0f;

	float GravityScale = 0.0f;
};

class SMatirialLibrary
{
public:
	void Add(std::string MaterialTag, const std::shared_ptr<FMaterial>& InMaterial);
	const FMaterial& Get(std::string MaterialTag);

private:
	std::unordered_map<std::string, std::shared_ptr<FMaterial>> MaterialTable;

};
