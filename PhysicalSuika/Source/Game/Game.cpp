#include "pch.h"

#include "Systems/Engine.h"
#include "Game/Game.h"
#include "Game/Cannon.h"
#include "Game/Clock.h"
#include "Game/Star.h"


SGame::SGame(int InWidth, int InHeight)
	: SceneWidth(InWidth)
	, SceneHeight(InHeight)
{
	AddEntity<ECannon>(SceneWidth / 2.f, SceneHeight - 50.f);
	AddEntity<EClock>(700.f, 100.f, 10.f);

	AddEntity<EStar>(400.f, 400.f, 10.f);
}

SGame::~SGame()
{
	for (Entity* Ent : Actors)
	{
		delete Ent;
	}

	Actors.clear();
}

void SGame::CullEntities()
{
	auto Range = std::remove_if(Actors.begin(), Actors.end(),
		[](Entity* Actor)
		{
			if (Actor->IsPendingDelete())
			{
				delete Actor;
				return true;
			}

			return false;
		});

	Actors.erase(Range, Actors.end());
}

void SGame::Tick()
{
	for (int i = 0; i < Actors.size(); i++)
	{
		Actors[i]->Tick();
	}
}
