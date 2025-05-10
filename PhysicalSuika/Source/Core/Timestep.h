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
		return std::chrono::duration<float, std::milli>(CurrentPoint - TimePoint).count(); 
	}

	inline float GetLapsedSeconds() const
	{
		auto CurrentPoint = std::chrono::high_resolution_clock::now();
		return std::chrono::duration<float>(CurrentPoint - TimePoint).count();
	}

private:
	std::chrono::high_resolution_clock::time_point TimePoint;

};

class STimestep
{
public:
	STimestep(float InTargetFPS, float InMaxFrametime)
		: TargetFPS(InTargetFPS)
		, Frametime(1.f / InTargetFPS)
		, MaxFrametime(InMaxFrametime)
		, DeltaTime(0.0f)
	{
	}

	inline float FrameStart()
	{
		DeltaTime = glm::min(Timer.GetLapsedSeconds(), MaxFrametime);
		Timer.Restart();

		return DeltaTime;
	}

	inline float GetDeltaTime() const { return DeltaTime; }

private:
	const float TargetFPS;
	const float Frametime;
	const float MaxFrametime;

	STimer Timer;
	float DeltaTime;  // In seconds
};
