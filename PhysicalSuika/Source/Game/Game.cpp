#include "pch.h"

#include "Systems/Engine.h"
#include "Game/Game.h"
#include "Game/Camera.h"
#include "Game/Arbiter.h"
#include "Game/Glass.h"
#include "Game/Fruit.h"
#include "Game/Hand.h"


SGame::SGame()
	: Top(17.0f)
	, Bottom(-1.5f)
	, Left(-14.0f)
	, Right(14.0f)
{
	Camera = AddEntity<ACamera>(glm::vec2{ 0.f, 8.f }, 1.0f, 10.0f);

	Arbiter = AddEntity<AArbiter>(glm::vec2{ 0.f, 0.f });
	AddEntity<AGlass>(glm::vec2{ 0.f, 0.f });
	AddEntity<AHand>(glm::vec2{ 0.f, 16.0f });
}

SGame::~SGame()
{
	Camera.reset();
	Arbiter.reset();

	Actors.Clear();
}

bool SGame::ClipOutOfBoundaries(StdShared<AActor> Actor) const
{
	glm::vec2 Pos = Actor->GetTransform().GetPos();

	return Pos.y > Top || Pos.y < Bottom || Pos.x < Left || Pos.x > Right;
}

void SGame::CullEntities()
{
	for (auto It = Actors.begin(); It != Actors.end(); It++)
	{
		StdShared<AActor> Actor = *It;
		if (Actor->IsPendingDelete() || ClipOutOfBoundaries(Actor))
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
