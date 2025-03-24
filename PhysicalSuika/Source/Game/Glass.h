#pragma once

#include "Actor.h"

struct CGeometry;
struct CollisionBox;

class AGlass : public AActor
{
public:
	AGlass(float InX, float InY);
	virtual ~AGlass() override;

	void Tick();

private:
	CGeometry* Geo;

	CollisionBox* Left;
	CollisionBox* Right;
	CollisionBox* Bottom;

};