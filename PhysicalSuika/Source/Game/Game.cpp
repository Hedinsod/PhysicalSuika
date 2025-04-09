#include "pch.h"

#include "Systems/Engine.h"
#include "Game/Game.h"
#include "Game/Actor.h"
#include "Game/Glass.h"
#include "Game/Fruit.h"
#include "Game/Hand.h"
#include "Renderer/Camera.h"
#include "Renderer/Renderer.h"

// #include <glm/gtc/matrix_transform.hpp>

SGame::SGame()
	: Top(17.0f)
	, Bottom(-1.5f)
	, Left(-14.0f)
	, Right(14.0f)
{
	// 800x600 -> 240x180
	Camera = std::make_shared<ACamera>(Top, Left, Bottom, Right, glm::vec2(0.f, 0.f));

	AddEntity<AGlass>(glm::vec2{ 0.f, 0.f });
	AddEntity<AHand>(glm::vec2{ 0.f, 16.0f });
}

SGame::~SGame()
{
/*
	for (std::shared_ptr<AActor> Ent : Actors)
	{
		delete Ent;
	}
*/
	Actors.Clear();
}

bool SGame::ClipOutOfBoundaries(std::shared_ptr<AActor> Actor)
{
	glm::vec2 Pos = Actor->GetTransform().GetPos();

	return Pos.y > Top || Pos.y < Bottom || Pos.x < Left || Pos.x > Right;
}

void SGame::CullEntities()
{
	for (auto It = Actors.begin(); It != Actors.end(); It++)
	{
		std::shared_ptr<AActor> Actor = *It;
		if (Actor->IsPendingDelete() || ClipOutOfBoundaries(Actor))
		{
			Actors.Remove(It);
		}
	}
}

void SGame::Tick(float DeltaTimeMs)
{
	for (std::shared_ptr<AActor> Actor : Actors)
	{
		Actor->Tick(DeltaTimeMs);
	}
}
