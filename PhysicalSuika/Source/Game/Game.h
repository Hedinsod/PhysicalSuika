#pragma once

#include "Core/Utility.h"
#include "Core/Containers/SparseArray.h"

class AActor;
class ACamera;

class SGame
{
public:
	SGame();
	~SGame();

	template <class TEntity, class... Args>
	void AddEntity(Args... args);
	bool ClipOutOfBoundaries(AActor* Actor);
	void CullEntities();

	void Tick();

	// ???
	float GetWidth() const { return Right - Left; }
	float GetHeight() const { return Top - Bottom; }

	std::shared_ptr<ACamera> GetCamera()
	{
		return Camera;
	};

	float Top, Bottom, Left, Right;

private:
	//std::vector<AActor*> Actors;
	TSparseArray<AActor*> Actors;

	std::shared_ptr<ACamera> Camera;

};

template <class TEntity, class... Args>
void SGame::AddEntity(Args... args)
{
	TEntity* NewEntity = new TEntity(args...);
	GAssert(NewEntity);
	Actors.Push(NewEntity);
}

extern SGame* GGame;
