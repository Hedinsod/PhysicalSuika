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
	inline float GetLapsedMilliseconds() const
	{
		auto CurrentPoint = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::microseconds>(CurrentPoint - TimePoint).count() * 0.001f;
	}

	inline float GetLapsedSeconds() const
	{
		auto CurrentPoint = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::milliseconds>(CurrentPoint - TimePoint).count() * 0.001f;
	}

private:
	std::chrono::steady_clock::time_point TimePoint;

};

class STimestep
{
public:
	STimestep(float InTargetFPS, float InMaxFrametime)
		: TargetFPS(InTargetFPS)
		, Frametime(1.f / InTargetFPS)
		, MaxFrametime(InMaxFrametime)
		, DeltaTime(0)
	{
	}

	inline float FrameStart()
	{
		DeltaTime = glm::min(Timer.GetLapsedSeconds(), MaxFrametime);
		Timer.Restart();

		return DeltaTime;
	}

	/*
	float GetStep()
	{
		return glm::min(FrametimeMs, DeltaTimeMs);
	}
	bool Update()
	{
		DeltaTimeMs -= FrametimeMs;
		return DeltaTimeMs > FrametimeMs;
	}
	*/

private:
	const float TargetFPS;
	const float Frametime;
	const float MaxFrametime;

	STimer Timer;
	float DeltaTime;  // In seconds
};
