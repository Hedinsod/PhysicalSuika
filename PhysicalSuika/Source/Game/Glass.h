#pragma once

#include "Actor.h"

class CGeometry;
struct CollisionBox;

class AGlass : public AActor
{
public:
	AGlass(glm::vec2 InPos);
	virtual ~AGlass() override;

	void Tick();

private:
	CGeometry* Geo;

	CollisionBox* Left;
	CollisionBox* Right;
	CollisionBox* Bottom;

};