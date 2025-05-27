#pragma once

#include "Core/Utility.h"
#include "Core/Containers/SparseArray.h"

class AActor;
class ACamera;
class AArbiter;
class AFruit;

struct FRect
{
	FRect(float InTop, float InBottom, float InLeft, float InRight)
		: Top(InTop)
		, Bottom(InBottom)
		, Left(InLeft)
		, Right(InRight)
	{ }

	float Top;
	float Bottom;
	float Left;
	float Right;
};

// Effectively it is Scene class
// Stores all actors, adds and removes them
class SGame
{
public:
	SGame();
	SGame(const SGame&) = delete;
	~SGame();

	// Adding actors
	template <class TEntity, class... Args>
	StdShared<TEntity> AddEntity(Args... args);

	// And deleting
	void CullEntities();

	// Update logic
	void Tick(float DeltaTimeMs);

	StdShared<ACamera> GetCamera()
	{
		return Camera;
	};

	AArbiter& GetArbiter()
	{
		return *Arbiter;
	};

	const FRect& GetGameZone()
	{
		return GameZone;
	}

	std::vector</*weak pointer*/ AFruit*> GetFruits();
	
private:
	TSparseArray<StdShared<AActor>> Actors;

	StdShared<ACamera> Camera;
	StdShared<AArbiter> Arbiter;

	// Valid game field, everything out of it - delete and finish game
	FRect GameZone;
};

template <class TEntity, class... Args>
StdShared<TEntity> SGame::AddEntity(Args... args)
{
	StdShared<TEntity> NewEntity = MakeShared<TEntity>(std::forward<Args>(args)...);
	GEnsure(NewEntity, "Can not create entity");
	Actors.Push(NewEntity);

	NewEntity->RegisterGame(this);

	return NewEntity;
}
