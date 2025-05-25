#pragma once
#include "Systems/Transform.h"


class SGame;

class AActor
{
public:
	AActor(const glm::vec2& InPos)
		: Trans(InPos)
		, bPendingDelete(false)
		, Game(nullptr)
	{ }
	virtual ~AActor() = default;

	void RegisterGame(SGame* InGame) { Game = InGame; }
	SGame* GetGame() const { return Game; }

	virtual void Tick(float DeltaTime) {}

	virtual void Delete() { bPendingDelete = true; }
	bool IsPendingDelete() const { return bPendingDelete; }

	const CTransform& GetTransform() const { return Trans; }
	CTransform& GetTransform() { return Trans; }

protected:
	CTransform Trans;

	bool bPendingDelete;

private:
	SGame* Game;

};




