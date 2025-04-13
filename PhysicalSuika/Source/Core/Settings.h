#pragma once

#include <cstdint>

// Global app/engine settings
struct FSettings
{
	int32_t WindowWidth = 800;
	int32_t WindowHeight = 600;

	float TargetFPS = 60.f;
	float MaxFrametimeMs = 200.f;
};
