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

/*
struct FPoint
{
	float x;
	float y;

	FPoint operator+(const FPoint& Other);
	FPoint& operator+=(const FPoint& Other);
	FPoint operator-(const FPoint& Other);
	FPoint& operator-=(const FPoint& Other);

	FPoint& operator*=(float Other);
};
*/

typedef std::shared_ptr<SGfxShader> SGfxShaderPtr;
typedef std::shared_ptr<SGfxVertexBuffer> SGfxVertexBufferPtr;
typedef std::shared_ptr<SGfxIndexBuffer> SGfxIndexBufferPtr;
typedef std::shared_ptr<SGfxVertexData> SGfxVertexDataPtr;

enum class EGfxApi
{
	None = 0,
	WinApi,
	OpenGL
};

class SGraphicsApi
{
public:
	virtual SGfxWindow* CreateGfxWindow(int InWidth, int InHeight, const std::string& InTitle) = 0;
	virtual SGfxShaderPtr CreateShader(const std::string& VertexSource, const std::string& FragmentSource) = 0;
	virtual SGfxVertexBufferPtr CreateVertexBuffer(const std::vector<float>& VertexData) = 0;
	virtual SGfxIndexBufferPtr CreateIndexBuffer(const std::vector<uint32_t>& IndexData) = 0;
	virtual SGfxVertexDataPtr CreateVertexData() = 0;

	virtual void DrawIndexed(const SGfxVertexDataPtr& VA) = 0;
	virtual void SetClearColor(const FColorRGB& InColor) = 0;
	virtual void Clear() = 0;
};

class SGraphics
{
public:
	static void Init(EGfxApi InApi);

	// Fabrics
	inline static SGfxWindow* CreateGfxWindow(int InWidth, int InHeight, const std::string& InTitle)
	{
		return Api->CreateGfxWindow(InWidth, InHeight, InTitle);
	}
	inline static SGfxShaderPtr CreateShader(const std::string& VertexSource, const std::string& FragmentSource)
	{
		return Api->CreateShader(VertexSource, FragmentSource);
	}
	inline static SGfxVertexBufferPtr CreateVertexBuffer(const std::vector<float>& VertexData)
	{
		return Api->CreateVertexBuffer(VertexData);
	}
	inline static SGfxIndexBufferPtr CreateIndexBuffer(const std::vector<uint32_t>& IndexData)
	{
		return Api->CreateIndexBuffer(IndexData);
	}
	inline static SGfxVertexDataPtr CreateVertexData()
	{
		return Api->CreateVertexData();
	}

	// Render Commands
	inline static void DrawIndexed(const SGfxVertexDataPtr& VertexData)
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

