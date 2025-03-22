#include "pch.h"
#include "Engine.h"

Engine* Engine::Instance = nullptr;

void Engine::Init(SGfxContext* InContext)
{
	Instance = new Engine;

	Instance->Collision = std::make_unique<SCollisionCheck>();
	GAssert(Instance->Collision);

	Instance->Graphics = std::make_unique<SGraphics>(InContext);
	GAssert(Instance->Graphics);
}