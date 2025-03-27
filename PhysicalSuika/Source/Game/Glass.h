#pragma once

#include "Actor.h"

class CGeometry;
class CPhysicsComp;

class AGlass : public AActor
{
public:
	AGlass(glm::vec2 InPos);
	virtual ~AGlass() override;

	void Tick();

private:
	CGeometry* Geo;

	CPhysicsComp* Left;
	CPhysicsComp* Right;
	CPhysicsComp* Bottom;

};