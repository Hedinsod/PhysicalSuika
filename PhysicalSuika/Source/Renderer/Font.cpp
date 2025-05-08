#include "pch.h"
#include "Font.h"
#include "Graphics/Graphics.h"

#include "stb_truetype.h"


StdShared<SGfxTexture> SFont::Prepare(const std::string& FontPath)
{
	std::string FontFile = Utility::LoadFileToString(FontPath);
	GAssert(FontFile.length() > 0);

	// transforming <const char*> to const <const unsigned char*> because of loGic.
    const unsigned char* FontBuffer = reinterpret_cast<const unsigned char*>(FontFile.c_str());

	stbtt_fontinfo Font;
	if (!stbtt_InitFont(&Font, FontBuffer, stbtt_GetFontOffsetForIndex(FontBuffer, /* index */ 0)))
	{
		Log::Log("Failed to initialize font\n");
		return nullptr;
	}

	// Temp buffers
	uint8_t* AlphaBitmap = new uint8_t[AtlasSize * AtlasSize];
	uint32_t* RgbaBitmap = new uint32_t[AtlasSize * AtlasSize];
	stbtt_bakedchar CharData[CharNum];

	stbtt_BakeFontBitmap(
		FontBuffer,   // font location
		0,            // offset
		PixelHeight,  // pixel size
		AlphaBitmap,  // pointer to memspace
		AtlasSize,    // width
		AtlasSize,    // height
		StartCode,    // start symbol
		CharNum,      // end symbol
		CharData);

	for (int32_t PixelIdx = 0; PixelIdx < AtlasSize * AtlasSize; PixelIdx++)
	{
		RgbaBitmap[PixelIdx] = 0x00ffffff | ((AlphaBitmap[PixelIdx] << 24) /* & 0xff000000 */);
	}

	constexpr uint32_t TextureFormatRGBA = 0x1908;

	StdScoped<SGfxTextureFactory> TexFactory = SGraphics::GetTextureFactory();
	StdShared<SGfxTexture> FontTexture = TexFactory->CreateFromData(RgbaBitmap, AtlasSize, AtlasSize, TextureFormatRGBA);

	delete[] AlphaBitmap;
	delete[] RgbaBitmap;

	PrepareQuads(CharData);

	return FontTexture;
}

void SFont::PrepareQuads(const stbtt_bakedchar* CharData)
{
	stbtt_aligned_quad AlignedQuad;
	float FakeX = 0.0f;
	float FakeY = 0.0f;

	for (int32_t CharIndex = 0; CharIndex < CharNum; CharIndex++)
	{
		FakeX = 0.0f;
		FakeY = 0.0f;

		stbtt_GetBakedQuad(CharData, AtlasSize, AtlasSize, CharIndex, &FakeX, &FakeY, &AlignedQuad, 1);  // 1 = opengl & d3d10+

		FSymbolQuad& Quad = SymbolQuads[CharIndex];

		float Height = AlignedQuad.y1 - AlignedQuad.y0;

		Quad.Vertices.emplace_back(AlignedQuad.x0 / PixelHeight, 0.0f);
		Quad.Vertices.emplace_back(AlignedQuad.x1 / PixelHeight, 0.0f);
		Quad.Vertices.emplace_back(AlignedQuad.x1 / PixelHeight, Height / PixelHeight);
		Quad.Vertices.emplace_back(AlignedQuad.x0 / PixelHeight, Height / PixelHeight);

		Quad.UVs.emplace_back(AlignedQuad.s0, AlignedQuad.t1);
		Quad.UVs.emplace_back(AlignedQuad.s1, AlignedQuad.t1);
		Quad.UVs.emplace_back(AlignedQuad.s1, AlignedQuad.t0);
		Quad.UVs.emplace_back(AlignedQuad.s0, AlignedQuad.t0);

		Quad.Advance = FakeX / PixelHeight;
	}
}

const FSymbolQuad& SFont::GetSymbolUV(char CharCode)
{
	uint8_t CharIndex = CharCode - StartCode;
	GAssert(CharIndex >= 0 && CharIndex < CharNum);

	return SymbolQuads[CharIndex];
}
