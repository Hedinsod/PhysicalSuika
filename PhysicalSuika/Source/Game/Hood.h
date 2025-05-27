#pragma once

#include "Actor.h"
#include "Physics/RigidBodyComp.h"
#include "Renderer/GeometryHandls.h"


// Checks if any fruit leaves the glass and asks Arbiter to finish the game
class AHood : public AActor
{
public:
	AHood(glm::vec2 InPos);
	virtual ~AHood() override;

	void OnCollision(AActor* Other);

private:
	CBodyHandle Trigger;
	FGeometryHandle GeoHandle;

};
