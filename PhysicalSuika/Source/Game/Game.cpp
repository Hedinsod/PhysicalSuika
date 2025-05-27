#include "pch.h"

#include "Systems/Engine.h"
#include "Game/Game.h"
#include "Game/Camera.h"
#include "Game/Arbiter.h"
#include "Game/Glass.h"
#include "Game/Fruit.h"
#include "Game/Hand.h"
#include "Game/Hood.h"


SGame::SGame()
	: GameZone(21.0f, -1.5f, -14.0f, 14.0f)
{
	Camera = AddEntity<ACamera>(glm::vec2{ 0.0f, 10.0f }, 1.0f, 11.0f);

	Arbiter = AddEntity<AArbiter>(glm::vec2{ 0.0f, 0.0f });

	AddEntity<AGlass>(glm::vec2{ 0.0f, 0.0f });
	AddEntity<AHood>(glm::vec2{ 0.0f, 16.5f });
	AddEntity<AHand>(glm::vec2{ 0.0f, 19.0f });
}

SGame::~SGame()
{
	Camera.reset();
	Arbiter.reset();

	Actors.Clear();
}

void SGame::CullEntities()
{
	for (auto It = Actors.begin(); It != Actors.end(); It++)
	{
		StdShared<AActor> Actor = *It;
		if (Actor->IsPendingDelete())
		{
			Actors.Remove(It);
		}
	}
}

void SGame::Tick(float DeltaTime)
{
	for (StdShared<AActor> Actor : Actors)
	{
		Actor->Tick(DeltaTime);
	}
}

std::vector<AFruit*> SGame::GetFruits()
{
	std::vector<AFruit*> FruitsArray;

	for (StdShared<AActor>& Actor : Actors)
	{
		AFruit* Fruit = dynamic_cast<AFruit*>(Actor.get());

		if (Fruit)
		{
			FruitsArray.push_back(Fruit);
		}
	}

	return FruitsArray;
}
