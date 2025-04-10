#pragma once

#include "Core/Utility.h"
#include "Core/Containers/SparseArray.h"

class AActor;
class ACamera;

// Effectively it is Scene class
// Stores all actors, adds and removes them
// 
class SGame
{
public:
	SGame();
	~SGame();

	// Adding and deleting actors
	template <class TEntity, class... Args>
	StdShared<TEntity> AddEntity(Args... args);
	bool ClipOutOfBoundaries(StdShared<AActor> Actor);
	void CullEntities();

	void Tick(float DeltaTimeMs);

	StdShared<ACamera> GetCamera()
	{
		return Camera;
	};

private:
	TSparseArray<StdShared<AActor>> Actors;

	StdShared<ACamera> Camera;

	// for clipping
	float Top, Bottom, Left, Right;

};

template <class TEntity, class... Args>
StdShared<TEntity> SGame::AddEntity(Args... args)
{
	StdShared<TEntity> NewEntity = MakeShared<TEntity>(std::forward<Args>(args)...);
	GAssert(NewEntity);
	Actors.Push(NewEntity);

	NewEntity->RegisterGame(this);

	return NewEntity;
}
