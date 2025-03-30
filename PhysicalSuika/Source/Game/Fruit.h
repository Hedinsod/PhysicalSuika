#pragma once

#include "Actor.h"

class CGeometry;
class CPhysicsComp;

class AFruit : public AActor
{
public:
	AFruit(glm::vec2 InPos);
	virtual ~AFruit() override;

	void Tick();

private:
	CGeometry* Geo;

	CPhysicsComp* Box;

	bool bStop = false;
};
