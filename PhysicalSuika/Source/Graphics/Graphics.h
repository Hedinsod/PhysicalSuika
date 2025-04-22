#pragma once

#include "Types.h"
#include "Core/SmartPointers.h"

#include "Graphics/GfxWindow.h"
#include "Graphics/GfxShader.h"
#include "Graphics/GfxBuffers.h"
#include "Graphics/GfxTexture.h"

#include <string>


enum class EGfxApi
{
	None = 0,
	OpenGL
};

class SGraphicsApi
{
public:
	virtual ~SGraphicsApi() = default;

	// Creats window with render context
	virtual StdScoped<SGfxWindow> CreateGfxWindow(uint32_t InWidth, uint32_t InHeight, const std::string& InTitle) = 0;

	// Factories of gfx objects 
	virtual StdScoped<SGfxShaderFactory> GetShaderFactory() = 0;
	virtual StdScoped<SGfxBufferFactory> GetBufferFactory() = 0;
	virtual StdScoped<SGfxTextureFactory> GetTextureFactory() = 0;


	// List of render commands
	virtual void DrawIndexed(uint32_t IndexCount) = 0;
	virtual void SetClearColor(const FColorRGB& InColor) = 0;
	virtual void Clear() = 0;

};

class SGraphics
{
public:
	static void Init(EGfxApi InApi);
	static void Shutdown()
	{
		delete Api;
	}

	// Creating window and gfx context
	inline static StdScoped<SGfxWindow> CreateGfxWindow(uint32_t InWidth, uint32_t InHeight, const std::string& InTitle)
	{
		return Api->CreateGfxWindow(InWidth, InHeight, InTitle);
	}

	// Factories
	inline static StdScoped<SGfxShaderFactory> GetShaderFactory()
	{
		return Api->GetShaderFactory();
	}
	inline static StdScoped<SGfxBufferFactory> GetBufferFactory()
	{
		return Api->GetBufferFactory();
	}
	inline static StdScoped<SGfxTextureFactory> GetTextureFactory()
	{
		return Api->GetTextureFactory();
	}

	// Render Commands
	inline static void DrawIndexed(uint32_t IndexCount)
	{
		return Api->DrawIndexed(IndexCount);
	}
	inline static void SetClearColor(const FColorRGB& InColor)
	{
		return Api->SetClearColor(InColor);
	}
	inline static void Clear()
	{
		return Api->Clear();
	}

	inline static SGraphicsApi& GetGraphicsApi() { return *Api; }

private:
	static EGfxApi CurrentApi;
	static SGraphicsApi* Api;

};
