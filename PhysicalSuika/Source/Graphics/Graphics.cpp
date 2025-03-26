#include "pch.h"
#include "Graphics.h"
#include "Platform/WinAPI/WinApiGraphics.h"
#include "Platform/OpenGL/OpenGLGraphics.h"

FPoint FPoint::operator+(const FPoint& Other)
{
	FPoint Result(*this);

	Result.x += Other.x;
	Result.y += Other.y;

	return Result;
}

FPoint& FPoint::operator+=(const FPoint& Other)
{
	x += Other.x;
	y += Other.y;

	return *this;
}

FPoint FPoint::operator-(const FPoint& Other)
{
	FPoint Result(*this);

	Result.x -= Other.x;
	Result.y -= Other.y;

	return Result;
}

FPoint& FPoint::operator-=(const FPoint& Other)
{
	x -= Other.x;
	y -= Other.y;

	return *this;
}

FPoint& FPoint::operator*=(float Other)
{
	x *= Other;
	y *= Other;

	return *this;
}

// ***********************************************************************************
EGfxApi SGraphics::CurrentApi = EGfxApi::None;
SGraphicsApi* SGraphics::Api = nullptr;

void SGraphics::Init(EGfxApi InApi)
{
	CurrentApi = InApi;
	switch (CurrentApi)
	{
	case EGfxApi::WinApi: Api = new SWinApiGraphics; break;
	case EGfxApi::OpenGL: Api = new SOpenGLGraphics; break;

	default: GAssert(false); break;
	}
}
