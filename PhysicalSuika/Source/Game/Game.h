#pragma once

#include "Core/Utility.h"
#include <vector>

class Entity;

class SGame
{
public:
	SGame(int InWidth, int InHeight);
	~SGame();

	template <class TEntity, class... Args>
	void AddEntity(Args... args);
	void CullEntities();

	void Tick();

	int GetWidth() const { return SceneWidth; }
	int GetHeight() const { return SceneHeight; }

private:
	const int SceneWidth;
	const int SceneHeight;
	
	std::vector<Entity*> Actors;
};

template <class TEntity, class... Args>
void SGame::AddEntity(Args... args)
{
	TEntity* NewEntity = new TEntity(args...);
	GAssert(NewEntity);
	Actors.push_back(NewEntity);
}

extern SGame* GGame;
