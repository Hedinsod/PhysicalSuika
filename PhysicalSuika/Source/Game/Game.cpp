#include "pch.h"

#include "Systems/Engine.h"
#include "Game/Game.h"
#include "Game/Actor.h"
#include "Game/Glass.h"
#include "Game/Fruit.h"
#include "Game/Hand.h"
#include "Renderer/Camera.h"
#include "Renderer/Renderer.h"


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
	for (AActor* Ent : Actors)
	{
		delete Ent;
	}

	Actors.clear();
}

void SGame::CullEntities()
{
	auto FwdIt = std::remove_if(Actors.begin(), Actors.end(), [](AActor* Actor){
		if (Actor->IsPendingDelete())
		{
			delete Actor;
			return true;
		}

		return false;
	});

	if (FwdIt != Actors.end())
		Actors.erase(FwdIt);
}

void SGame::Tick()
{
	for (int i = 0; i < Actors.size(); i++)
	{
		Actors[i]->Tick();
	}
}
