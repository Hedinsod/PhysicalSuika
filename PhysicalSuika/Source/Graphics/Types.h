#pragma once

#include <glm/glm.hpp>

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
		: ColorVec4(InR, InG, InB, 1.0f)
	{
	}
	FColorLinear(const FColorRGB& InColor)
		: ColorVec4(InColor.Red / 255.f,
			InColor.Green / 255.f,
			InColor.Blue / 255.f,
			1.0f)
	{
	}

	glm::vec4 ColorVec4;
};
