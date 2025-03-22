#pragma once
#include "Systems/Transform.h"


class Entity
{
public:
	Entity(float InX, float InY)
		: Trans(this, InX, InY)
	{
	}
	virtual ~Entity() = default;

	virtual void Tick() = 0;

	// Events
	bool IsPendingDelete() { return bPendingDelete; }
	virtual void OnCollide(const Entity* Opponent) {}
	virtual void OnOutOfBounds(bool bHorizontal, bool bVertical) {}
	// virtual void OnKeyPressed(...) {}

	const Transform& GetTransform() { return Trans; }

protected:
	Transform Trans;

	bool bPendingDelete = false;
};




