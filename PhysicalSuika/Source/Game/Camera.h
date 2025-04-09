#pragma once

#include "Actor.h"


class ACamera : public AActor
{
public:
	ACamera(glm::vec2 InPos, float AspectRatio, float Zoom);
	~ACamera();

	// If I'll ever need camera control
	// virtual void Tick(float DeltaTimeMs) override;

	inline glm::mat4x4 GetVP() const { return ProjView; }

	// Events
	void OnResize(int32_t NewWidth, int32_t NewHeight);

private:
	void UpdateProjView();

	glm::mat4x4 Projection;
	glm::mat4x4 ProjView;

	float AspectRatio;
	float Zoom;

};
