#pragma once

#include <string>
namespace Log
{
	void Log(const std::string& Message);
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

#if defined(BUILD_DEBUG) || defined (BUILD_DEVELOPMENT)
#define GAssert( Condition )                                                        \
	{                                                                               \
		if (!( Condition ))                                                         \
		{                                                                           \
			GBreakPoint;                                                            \
		}                                                                           \
	}

#define GAssertLog( Condition, Message )                                            \
	{                                                                               \
		if (!( Condition ))                                                         \
		{                                                                           \
			Log::Log( Message );                                                    \
			GBreakPoint;                                                            \
		}                                                                           \
	}
#else
#define GAssert( ... ) ( (void)0 )
#define GAssertLog( ... ) ( (void)0 )
#endif
