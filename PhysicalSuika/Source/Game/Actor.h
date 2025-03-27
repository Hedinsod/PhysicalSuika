#pragma once
#include "Renderer/Transform.h"


class AActor
{
public:
	AActor(glm::vec2 InPos, const char* Name)
		: tag(Name)
	{
		Trans.SetPos(InPos);
	}
	virtual ~AActor() = default;

	virtual void Tick() = 0;

	// "Events"
	bool IsPendingDelete() { return bPendingDelete; }
	virtual void OnCollide(AActor& Opponent) {}
	virtual void OnOutOfBounds(bool bHorizontal, bool bVertical)
	{
		this->bPendingDelete = true;
	}

	const CTransform& GetTransform() const { return Trans; }
	std::string tag;
protected:
	CTransform Trans;

	bool bPendingDelete = false;

	
};




