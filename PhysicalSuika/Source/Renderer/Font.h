#pragma once

class SGfxTexture;

#include "Renderer/GeometryHandls.h"
#include "stb_truetype.h"

#include <array>

struct FSymbolQuad
{
	std::vector<glm::vec2> Vertices;
	std::vector<glm::vec2> UVs;

	float Advance = 0.0f;
};

class SFont final
{
public:
	SFont() = default;
	~SFont() = default;

	StdShared<SGfxTexture> Prepare(const std::string& FontPath);
	void PrepareQuads(const stbtt_bakedchar* CharData);

	const FSymbolQuad& GetSymbolUV(char CharCode);

private:
	inline static const float PixelHeight = 64.0f;
	static const uint32_t StartCode = 32;
	static const uint32_t CharNum = 96;
	static const uint32_t AtlasSize = 512;

	std::array<FSymbolQuad, CharNum> SymbolQuads;
};
