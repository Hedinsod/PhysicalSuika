#pragma once
#include "Entity.h"

struct CGeometry;
struct CollisionBox;

class EStar : public Entity
{
public:
	EStar(float InX, float InY, float InScale);
	virtual ~EStar() override;

	void Tick();

	virtual void OnCollide(const Entity* Opponent) override;
	virtual void OnOutOfBounds(bool bHorizontal, bool bVertical) override;

private:
	static constexpr int BaseSide = 5;
	float Scale;
	float Angle;

	FPoint Velocity;

	CollisionBox* Collider;
	CGeometry* Box;

	CGeometry* Rays[4];
};