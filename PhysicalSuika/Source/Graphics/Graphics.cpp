#include "pch.h"
#include "Graphics.h"
#include "Platform/WinAPI/WinApiWindow.h"
#include "Platform/OpenGL/OpenGLWindow.h"

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
EGfxApi SGraphics::CurrentApi = EGfxApi::WinApi;

SGfxWindow* SGraphics::CreateGfxWindow(int InWidth, int InHeight, const std::string& InTitle)
{
	switch (CurrentApi)
	{
	case EGfxApi::WinApi: return new SWinApiWindow(InWidth, InHeight, InTitle); break;
	case EGfxApi::OpenGL: return new SOpenGLWindow(InWidth, InHeight, InTitle); break;

	default: GAssert(false); break;
	}

	return nullptr;
}
