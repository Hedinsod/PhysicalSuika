#pragma once

#include "Actor.h"

class CGeometry;

class AHand : public AActor
{
public:
	AHand(glm::vec2 InPos);
	virtual ~AHand() override;

	void Tick();

	virtual void OnCollide(const AActor* Opponent) {}
	

private:
	CGeometry* Geo;

	const int32_t SpawnCooldownStart = 50;
	int32_t SpawnCooldown = 0;
};