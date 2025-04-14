#include "pch.h"
#include "Math.h"

#include <random>


namespace Math
{
	int32_t GetRandom(int32_t Low, int32_t Up)
	{
		static std::random_device dev;
		static std::mt19937 rng;
		static std::uniform_int_distribution<std::mt19937::result_type> dist(0, 65565);

		int32_t Correction = Low < 0 ? 1 : 0;
		int32_t Random = dist(rng);

		return (Random % (Up - Low)) + Low + Correction;
	}
}
