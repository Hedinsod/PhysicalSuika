#pragma once

#include "Actor.h"
#include "Renderer/GeometryHandls.h"

class AFruit;
class CGeometry;

class AHand : public AActor
{
public:
	AHand(glm::vec2 InPos);
	virtual ~AHand() override;

	// Enable and disable input/preview/movement
	// for pause / gmae ending
	void Hold();
	void Release();

	virtual void Tick(float DeltaTime) override;

	void SpawnPreview(const glm::vec2& InSpawnPoint);

private:
	FGeometryHandle GeoHandle;

	StdShared<AFruit> Preview;
	float SpawnCooldown;

	float HandOffset;
	bool bPaused;
};