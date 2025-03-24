#pragma once

class SGfxWindow;

struct FColor
{
	int Red = 0;
	int Green = 0;
	int Blue = 0;
};

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

enum class EGfxApi
{
	WinApi = 0,
	OpenGL
};

class SGraphics
{
public:
	static SGfxWindow* CreateGfxWindow(int InWidth, int InHeight, const std::string& InTitle);

private:
	static EGfxApi CurrentApi;

};

