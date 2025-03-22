#pragma once

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

	FPoint& operator*=(float Other);
};
