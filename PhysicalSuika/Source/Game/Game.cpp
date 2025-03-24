#include "pch.h"

#include "Systems/Engine.h"
#include "Game/Game.h"
#include "Game/Actor.h"
#include "Game/Glass.h"
#include "Game/Fruit.h"
#include "Game/Hand.h"



SGame::SGame()
{
	AddEntity<AGlass>(400.f, 550.f);
	AddEntity<AHand>(400.f, 50.f);
	AddEntity<AFruit>(400.f, 350.f);
}

SGame::~SGame()
{
	for (AActor* Ent : Actors)
	{
		delete Ent;
	}

	Actors.clear();
}

void SGame::CullEntities()
{
	for (auto It = Actors.begin(); It != Actors.end(); It++)
	{
		AActor* Actor = *It;
		if (Actor->IsPendingDelete())
		{
			delete Actor;
			It = Actors.erase(It);
		}

		if (It == Actors.end()) break;
	}
}

void SGame::Tick()
{
	for (int i = 0; i < Actors.size(); i++)
	{
		Actors[i]->Tick();
	}
}
