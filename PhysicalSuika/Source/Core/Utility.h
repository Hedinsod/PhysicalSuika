#pragma once

namespace Utility
{
	//constexpr float FullCircle = 6.2831f;

	//get system time
	void GetTime(int& hr, int& min, int& sec);
	int GetRandom(int low, int up);
}

// general types
using Int32 = __int32;
using UInt32 = unsigned __int32;
using Int64 = __int64;
using UInt64 = unsigned __int64;

// 
#define GAssert(expr) { if(!(expr)) { __debugbreak(); } }
