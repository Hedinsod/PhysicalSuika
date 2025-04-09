#pragma once
#include "Renderer/Transform.h"


class SGame;

class AActor
{
public:
	AActor(glm::vec2 InPos)
	{
		Trans.SetPos(InPos);
	}
	virtual ~AActor() = default;

	void RegisterGame(SGame* InGame) { Game = InGame; }
	SGame* GetGame() { return Game; }

	virtual void Tick(float DeltaTime) {}

	// "Events"
	bool IsPendingDelete() { return bPendingDelete; }
	virtual void OnOutOfBounds(bool bHorizontal, bool bVertical)
	{
		this->bPendingDelete = true;
	}

	const CTransform& GetTransform() const { return Trans; }
	CTransform& GetTransform() { return Trans; }

protected:
	CTransform Trans;

	bool bPendingDelete = false;

private:
	SGame* Game = nullptr;

};




