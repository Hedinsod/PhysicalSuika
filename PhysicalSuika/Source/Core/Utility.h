#pragma once

#include <glm/glm.hpp>

namespace Utility
{
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

	void Log(const std::string& Message);

	// Reads file into std::string and returns it
	// If fails to open file prints in log and returns empty string
	std::string LoadFileToString(const std::string& Path);
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
namespace Utility
{
	void AssertLog(const char* condition, const char* fileName, int32_t lineNumber);
}
#define GAssert( Condition )                                                        \
	{                                                                               \
		if (!( Condition ))                                                         \
		{                                                                           \
			Utility::AssertLog( #Condition, __FILE__, (int32_t)__LINE__ );          \
			GBreakPoint;                                                            \
		}                                                                           \
	}
#else
#define GAssert( ... ) ( (void)0 )
#endif
