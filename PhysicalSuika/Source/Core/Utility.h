#pragma once

#include <glm/glm.hpp>

namespace Utility
{
	// Get system time
	void GetTime(int& hr, int& min, int& sec);

	// Get random number from range [low, up)
	int GetRandom(int low, int up);

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
}

// see https://github.com/scottt/debugbreak
#if defined( _MSC_VER )
#define GBreakPoint __debugbreak()
#elif defined( __GNUC__ ) || defined( __clang__ )
#define GBreakPoint __builtin_trap()
#else
// Unknown compiler
#include <assert.h>
#define GBreakPoint assert( 0 )
#endif

#if defined( _DEBUG )
//void Log(const char* condition, const char* fileName, int lineNumber); <-- some log function!
#define GAssert( condition )                                                                                    \
	do                                                                                                          \
	{                                                                                                           \
		if (!( condition )) /* { && Log( #condition, __FILE__, (int)__LINE__ ) ); GBreakPoint; } */             \
			GBreakPoint;                                                                                        \
	}                                                                                                           \
	while ( 0 )
#else
#define GAssert( ... ) ( (void)0 )
#endif
