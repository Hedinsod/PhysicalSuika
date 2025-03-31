#pragma once

#include "Actor.h"
#include "Physics/RigidBodyComp.h"


class CGeometry;

class AGlass : public AActor
{
public:
	AGlass(glm::vec2 InPos);
	virtual ~AGlass() override;

	void Tick();

private:
	CGeometry* Geo;

	CBodyHandle Left;
	CBodyHandle Right;
	CBodyHandle Bottom;

};