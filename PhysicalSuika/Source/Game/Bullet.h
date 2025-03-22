#pragma once

#include "Entity.h"
#include "Graphics/Types.h"

struct CollisionBox;
struct CGeometry;

class EBullet : public Entity
{
public:
	EBullet(float InX, float InY, float Angle);
	virtual ~EBullet() override;

	void Tick();

	virtual void OnCollide(const Entity* Opponent) override;
	virtual void OnOutOfBounds(bool bHorizontal, bool bVertical) override;

private:
	static constexpr float Speed = 6.0f;

	FPoint Velocity;

	CollisionBox* Collider;
	CGeometry* Geo;
};
