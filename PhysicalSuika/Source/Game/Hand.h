#pragma once

#include "Actor.h"
#include "Renderer/GeometryHandle.h"

class AFruit;
class CGeometry;

class AHand : public AActor
{
public:
	AHand(glm::vec2 InPos);
	virtual ~AHand() override;

	virtual void Tick(float DeltaTime) override;

	virtual void OnCollide(const AActor* Opponent) {}
	
private:
	const float HandSpeed = 0.1f;
	const glm::vec2 FruitOffset{0.f, -1.5f};

	StdShared<AFruit> Preview;
	float HandOffset = 0.0f;

	FGeometryHandle GeoHandle;

	const float SpawnCooldownStart = 1.0f;
	float SpawnCooldown = 0.0f;
};