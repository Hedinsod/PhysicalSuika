#include "pch.h"
#include "Engine.h"
#include "Core/Application.h"
#include "Graphics/GfxWindow.h"

Engine* Engine::Instance = nullptr;

void Engine::Init()
{
	Instance = new Engine;

	const FSettings& Settings = GApp->GetSettings();

	const float Frametime = 1.0f / Settings.TargetFPS;
	Instance->PhyScene = MakeScoped<SPhyScene>(Frametime, Settings.PhysicsSubStepsCount);
	GAssert(Instance->PhyScene);

	Instance->Graphics = MakeScoped<SGeometryPool>();
	GAssert(Instance->Graphics);

	Instance->LoadMaterials();
}

void Engine::Shutdown()
{
	delete Instance;
	Instance = nullptr;
}

void Engine::LoadMaterials()
{
	MatirialLibrary.Add("Default", MakeShared<FMaterial>());
	MatirialLibrary.Add("Berry", MakeShared<FMaterial>(/*.Density*/ 0.0955f, /*.Friction*/ 0.2f, /*.GravityScale*/ 1.0f, /*Color*/glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));
	MatirialLibrary.Add("Glass", MakeShared<FMaterial>(/*.Density*/    0.0f, /*.Friction*/ 0.4f, /*.GravityScale*/ 1.0f, /*Color*/glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
}
