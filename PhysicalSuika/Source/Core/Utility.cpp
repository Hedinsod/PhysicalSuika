#include "pch.h"
#include <random>
#include <iostream>

namespace Utility
{
	void GetTime(int& hr, int& min, int& sec)
	{
		SYSTEMTIME time;
		GetLocalTime(&time);
		hr = time.wHour;
		min = time.wMinute;
		sec = time.wSecond;
	}

	int GetRandom(int Low, int Up)
	{
		static std::random_device dev; 
		static std::mt19937 rng;
		static std::uniform_int_distribution<std::mt19937::result_type> dist(0, 65565);

		int Correction = Low < 0 ? 1 : 0;
		int Random = dist(rng);

		return (Random % (Up - Low)) + Low + Correction;
	}

#if defined( _DEBUG )
	void AssertLog(const char* Condition, const char* FileName, int32_t LineNumber)
	{
		std::cerr << "Assertion failed: " << Condition 
		          << " in " << FileName
		          << ", at " << LineNumber << std::endl;
	}
#endif

	void Log(const std::string& Message)
	{
		std::cerr << Message << std::endl;
	}
}
