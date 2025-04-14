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
	StdShared<AFruit> Preview;

	FGeometryHandle GeoHandle;

	const float SpawnCooldownStart = 1.0f;
	float SpawnCooldown = 0.0f;
};