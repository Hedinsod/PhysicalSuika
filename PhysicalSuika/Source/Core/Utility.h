#pragma once

namespace Utility
{
	// Get system time
	void GetTime(int& hr, int& min, int& sec);

	// Get ranbdom number from range [low, up)
	int GetRandom(int low, int up);
}

// 
#define GAssert(expr) { if(!(expr)) { __debugbreak(); } }
