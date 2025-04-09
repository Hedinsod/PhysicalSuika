#pragma once

#include <chrono>
#include <glm/glm.hpp>

class STimer
{
public:
	STimer()
	{
		Restart();
	}

	inline void Restart()
	{
		TimePoint = std::chrono::high_resolution_clock::now();
	}
	inline float GetLapsedMilliseconds()
	{
		auto CurrentPoint = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::microseconds>(CurrentPoint - TimePoint).count() * 0.001f;
	}

	inline float GetLapsedMsClamped(float MaxFrameTimeMs)
	{
		auto CurrentPoint = std::chrono::high_resolution_clock::now();
		float LapsedMs = std::chrono::duration_cast<std::chrono::microseconds>(CurrentPoint - TimePoint).count() * 0.001f;

		return LapsedMs < MaxFrameTimeMs ? LapsedMs : MaxFrameTimeMs;
	}

private:
	std::chrono::steady_clock::time_point TimePoint;

};

class STimestep
{
public:
	STimestep(float InTargetFPS, float InMaxFrametimeMs)
		: TargetFPS(InTargetFPS)
		, FrametimeMs(1000.f / InTargetFPS)
		, MaxFrametimeMs(InMaxFrametimeMs)
		, DeltaTimeMs(0)
	{
	}

	inline void FrameStart()
	{
		DeltaTimeMs = Timer.GetLapsedMsClamped(MaxFrametimeMs);
		Timer.Restart();
	}

	// Returns full delta time since last Start
	float GetFullStep()
	{
		return glm::min(MaxFrametimeMs, DeltaTimeMs);
	}

	float GetStep()
	{
		return glm::min(FrametimeMs, DeltaTimeMs);
	}

	bool Update()
	{
		DeltaTimeMs -= FrametimeMs;
		return DeltaTimeMs > FrametimeMs;
	}

private:
	float TargetFPS = 60.f;
	float FrametimeMs = 1000.f / TargetFPS;
	float MaxFrametimeMs = 200.f;

	STimer Timer;
	float DeltaTimeMs;
};
