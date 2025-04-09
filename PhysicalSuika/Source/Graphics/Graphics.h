#pragma once

#include <memory>
#include <string>

class SGfxWindow;
class SGfxShader;
class SGfxVertexBuffer;
class SGfxIndexBuffer;
class SGfxVertexData;

struct FColorRGB
{
	int Red = 0;
	int Green = 0;
	int Blue = 0;
};

struct FColorLinear
{
	FColorLinear() = default;
	FColorLinear(float InR, float InG, float InB)
		: Red(InR), Green(InG), Blue(InB)
	{
	}
	FColorLinear(const FColorRGB& InColor)
		: Red(InColor.Red / 255.f), Green(InColor.Green / 255.f), Blue(InColor.Blue / 255.f)
	{
	}

	float Red = 0;
	float Green = 0;
	float Blue = 0;
};

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
	virtual std::shared_ptr<SGfxShader> CreateShader(const std::string& VertexSource, const std::string& FragmentSource) = 0;
	virtual std::shared_ptr<SGfxVertexBuffer> CreateVertexBuffer(const std::vector<float>& VertexData) = 0;
	virtual std::shared_ptr<SGfxIndexBuffer> CreateIndexBuffer(const std::vector<uint32_t>& IndexData) = 0;
	virtual std::shared_ptr<SGfxVertexData> CreateVertexData() = 0;

	virtual void DrawIndexed(const std::shared_ptr<SGfxVertexData>& VA) = 0;
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
	inline static std::shared_ptr<SGfxShader> CreateShader(const std::string& VertexSource, const std::string& FragmentSource)
	{
		return Api->CreateShader(VertexSource, FragmentSource);
	}
	inline static std::shared_ptr<SGfxVertexBuffer> CreateVertexBuffer(const std::vector<float>& VertexData)
	{
		return Api->CreateVertexBuffer(VertexData);
	}
	inline static std::shared_ptr<SGfxIndexBuffer> CreateIndexBuffer(const std::vector<uint32_t>& IndexData)
	{
		return Api->CreateIndexBuffer(IndexData);
	}
	inline static std::shared_ptr<SGfxVertexData> CreateVertexData()
	{
		return Api->CreateVertexData();
	}

	// Render Commands
	inline static void DrawIndexed(const std::shared_ptr<SGfxVertexData>& VertexData)
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

private:
	static EGfxApi CurrentApi;
	static SGraphicsApi* Api;

};
