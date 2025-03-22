#pragma once

#include "Entity.h"

struct CGeometry;

class ECannon : public Entity
{
public:
	ECannon(float InX, float InY);
	virtual ~ECannon() override;

	void Tick();

private:
	static constexpr int Width = 10;
	static constexpr int Height = 30;
	static constexpr float AngleStep = 0.05f;
	static constexpr int ShootingCooldown = 5;

	float Angle;

	CGeometry* Geo;
};