#pragma once

#include "Actor.h"

class CGeometry;
class CPhysicsComp;

class AFruit : public AActor
{
public:
	AFruit(glm::vec2 InPos);
	virtual ~AFruit() override;
	virtual void OnCollide(AActor& Opponent) override;

	void Tick();

private:
	CGeometry* Geo;
	CGeometry* GeoBox;

	CPhysicsComp* Box;

	bool bStop = false;
};
