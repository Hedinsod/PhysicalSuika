#pragma once

#include "Actor.h"

class AFruit;
class CGeometry;

class AHand : public AActor
{
public:
	AHand(glm::vec2 InPos);
	virtual ~AHand() override;

	virtual void Tick(float DeltaTimeMs) override;

	virtual void OnCollide(const AActor* Opponent) {}
	
private:
	std::shared_ptr<AFruit> Preview;

	CGeometry* Geo;

	const float SpawnCooldownStart = 1000.0f;
	float SpawnCooldown = 0.0f;
};