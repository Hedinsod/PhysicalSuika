#include "pch.h"
#include "Engine.h"
#include "Core/Application.h"
#include "Graphics/GfxWindow.h"

Engine* Engine::Instance = nullptr;

void Engine::Init()
{
	Instance = new Engine;

	Instance->Collision = std::make_unique<SPhScene>(4);
	GAssert(Instance->Collision);

	SGfxContext* Context = GApp->GetWindow().GetGfxContext();
	Instance->Graphics = std::make_unique<SDraw>(Context);
	GAssert(Instance->Graphics);
}