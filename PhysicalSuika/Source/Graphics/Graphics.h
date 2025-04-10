#pragma once

#include "Types.h"
#include "Core/SmartPointers.h"
#include "GfxShader.h"
#include <string>

class SGfxWindow;

class SGfxVertexBuffer;
class SGfxIndexBuffer;
class SGfxVertexData;


enum class EGfxApi
{
	None = 0,
	WinApi,
	OpenGL
};

class SGraphicsApi
{
public:
	virtual ~SGraphicsApi() = default;

	virtual SGfxWindow* CreateGfxWindow(int InWidth, int InHeight, const std::string& InTitle) = 0;
	virtual StdScoped<SGfxShaderFactory> GetShaderFactory() = 0;

	virtual StdShared<SGfxVertexBuffer> CreateVertexBuffer(const std::vector<float>& VertexData) = 0;
	virtual StdShared<SGfxIndexBuffer> CreateIndexBuffer(const std::vector<uint32_t>& IndexData) = 0;
	virtual StdShared<SGfxVertexData> CreateVertexData() = 0;

	virtual void DrawIndexed(const StdShared<SGfxVertexData>& VA) = 0;
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

	// Fabrics
	inline static SGfxWindow* CreateGfxWindow(int InWidth, int InHeight, const std::string& InTitle)
	{
		return Api->CreateGfxWindow(InWidth, InHeight, InTitle);
	}

	inline static StdScoped<SGfxShaderFactory> GetShaderFactory()
	{
		return Api->GetShaderFactory();
	}

	inline static StdShared<SGfxVertexBuffer> CreateVertexBuffer(const std::vector<float>& VertexData)
	{
		return Api->CreateVertexBuffer(VertexData);
	}
	inline static StdShared<SGfxIndexBuffer> CreateIndexBuffer(const std::vector<uint32_t>& IndexData)
	{
		return Api->CreateIndexBuffer(IndexData);
	}
	inline static StdShared<SGfxVertexData> CreateVertexData()
	{
		return Api->CreateVertexData();
	}

	// Render Commands
	inline static void DrawIndexed(const StdShared<SGfxVertexData>& VertexData)
	{
		return Api->DrawIndexed(VertexData);
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
