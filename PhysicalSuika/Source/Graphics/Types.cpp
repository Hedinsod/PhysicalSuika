#include "pch.h"
#include "Graphics/Types.h"

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

FPoint& FPoint::operator*=(float Other)
{
	x *= Other;
	y *= Other;

	return *this;
}
