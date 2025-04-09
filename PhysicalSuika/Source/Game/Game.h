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
	std::shared_ptr<TEntity> AddEntity(Args... args);
	bool ClipOutOfBoundaries(std::shared_ptr<AActor> Actor);
	void CullEntities();

	void Tick(float DeltaTimeMs);

	// ???
	float GetWidth() const { return Right - Left; }
	float GetHeight() const { return Top - Bottom; }

	std::shared_ptr<ACamera> GetCamera()
	{
		return Camera;
	};

	float Top, Bottom, Left, Right;

private:
	TSparseArray<std::shared_ptr<AActor>> Actors;

	std::shared_ptr<ACamera> Camera;

};

template <class TEntity, class... Args>
std::shared_ptr<TEntity> SGame::AddEntity(Args... args)
{
	std::shared_ptr<TEntity> NewEntity = std::make_shared<TEntity>(args...);
	GAssert(NewEntity);
	Actors.Push(NewEntity);

	NewEntity->RegisterGame(this);

	return NewEntity;
}
