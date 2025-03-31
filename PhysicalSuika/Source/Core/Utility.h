#pragma once

namespace Utility
{
	// Get system time
	void GetTime(int& hr, int& min, int& sec);

	// Get ranbdom number from range [low, up)
	int GetRandom(int low, int up);
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
#define GAssert( condition )                                                                                                   \
	do                                                                                                                         \
	{                                                                                                                          \
		if (!( condition )) /* { && Log( #condition, __FILE__, (int)__LINE__ ) ); GBreakPoint; } */                            \
			GBreakPoint;                                                                                                       \
	}                                                                                                                          \
	while ( 0 )
#else
#define B2_ASSERT( ... ) ( (void)0 )
#endif
