#pragma once

#include <cstdint>

// Global app/engine settings
struct FSettings
{
	uint32_t WindowWidth = 800;
	uint32_t WindowHeight = 600;

	float TargetFPS = 60.f;
	float MaxFrametime = 0.2f;

	uint32_t PhysicsSubStepsCount = 4;

	std::string FontPath = "Content/Fonts/KOMIKAX_.ttf";
};
