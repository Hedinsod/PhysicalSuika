#pragma once
#include "Systems/Transform.h"


class AActor
{
public:
	AActor(float InX, float InY)
		: Trans(this, InX, InY)
	{
	}
	virtual ~AActor() = default;

	virtual void Tick() = 0;

	// "Events"
	bool IsPendingDelete() { return bPendingDelete; }
	virtual void OnCollide(const AActor* Opponent) {}
	virtual void OnOutOfBounds(bool bHorizontal, bool bVertical)
	{
		this->bPendingDelete = true;
	}

	const CTransform& GetTransform() { return Trans; }

protected:
	CTransform Trans;

	bool bPendingDelete = false;
};




