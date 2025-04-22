#pragma once

#include <unordered_map>
#include <string>
#include <memory>


class SGfxTexture;

struct FMaterial
{
	FMaterial(float InDensity, float InFriction, float InGravityScale, const glm::vec4& InColor, const StdShared<SGfxTexture>& InTexture)
		: Density(InDensity)
		, Friction(InFriction)
		, GravityScale(InGravityScale)
		, Color(InColor)
		, Texture(InTexture)
	{ }

	float Density = 0.0f;
	float Friction = 0.0f;

	float GravityScale = 0.0f;

	glm::vec4 Color{ 0.0f, 0.0f, 0.0f, 1.0f };

	StdShared<SGfxTexture> Texture;
};

class SMatirialLibrary
{
public:
	void Add(std::string MaterialTag, const StdShared<FMaterial>& InMaterial);
	const FMaterial& Get(std::string MaterialTag);

private:
	// TODO: Switch to storing FMaterial, not pointer
	std::unordered_map<std::string, StdShared<FMaterial>> MaterialTable;

};
