#pragma once

#include "Actor.h"

class CGeometry;
struct CollisionBox;

class AFruit : public AActor
{
public:
	AFruit(float InX, float InY);
	virtual ~AFruit() override;
	virtual void OnCollide(const AActor* Opponent) override;

	void Tick();

private:
	CGeometry* Geo;
	CollisionBox* Box;

	bool bStop = false;
};
