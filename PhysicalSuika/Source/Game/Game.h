#pragma once

#include "Core/Utility.h"
#include "Core/Containers/SparseArray.h"

class AActor;
class ACamera;
class AArbiter;

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
	bool ClipOutOfBoundaries(StdShared<AActor> Actor) const;
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

private:
	TSparseArray<StdShared<AActor>> Actors;

	StdShared<ACamera> Camera;
	StdShared<AArbiter> Arbiter;

	// for clipping
	float Top, Bottom, Left, Right;

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
