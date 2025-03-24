#pragma once

#include "Core/Utility.h"
#include <vector>

class AActor;

class SGame
{
public:
	SGame();
	~SGame();

	template <class TEntity, class... Args>
	void AddEntity(Args... args);
	void CullEntities();

	void Tick();

	// ???
	int GetWidth() const { return 800; }
	int GetHeight() const { return 600; }

private:
	const int SceneWidth = 10;
	const int SceneHeight = 10;
	
	std::vector<AActor*> Actors;
};

template <class TEntity, class... Args>
void SGame::AddEntity(Args... args)
{
	TEntity* NewEntity = new TEntity(args...);
	GAssert(NewEntity);
	Actors.push_back(NewEntity);
}

extern SGame* GGame;
