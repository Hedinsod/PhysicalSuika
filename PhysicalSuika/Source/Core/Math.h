#pragma once

#include <glm/glm.hpp>

// Functions that [somehow] are missed in glm
namespace Math
{
	// 2d cross product
	inline float cross2(const glm::vec2& v1, const glm::vec2& v2)
	{
		return v1.x * v2.y - v1.y * v2.x;
	}
	inline glm::vec2 cross2(const glm::vec2& v, float f)
	{
		return glm::vec2(f * v.y, -f * v.x);
	}
	inline glm::vec2 cross2(float f, const glm::vec2& v)
	{
		return glm::vec2(-f * v.y, f * v.x);
	}

	// Random number generator
	int32_t GetRandom(int32_t Low, int32_t Up);
}
