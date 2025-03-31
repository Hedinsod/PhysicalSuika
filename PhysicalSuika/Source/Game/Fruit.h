#pragma once

#include "Actor.h"
#include "Physics/RigidBodyComp.h"

class CGeometry;

class AFruit : public AActor
{
public:
	AFruit(glm::vec2 InPos);
	virtual ~AFruit() override;

	void Tick();

private:
	CGeometry* Geo;
	CBodyHandle Box;

	bool bStop = false;
};
