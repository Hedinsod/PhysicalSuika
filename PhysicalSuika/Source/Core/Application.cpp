#include "pch.h"

#include "Core/Application.h"
#include "Timestep.h"
#include "Graphics/GfxWindow.h"
#include "Graphics/Graphics.h"
#include "Systems/Engine.h"
#include "Game/Game.h"

#include "Renderer/Renderer.h"

SGame* GGame = nullptr;

Application::Application()
{
}

Application::~Application()
{
	MainWindow->Destroy();
}

void Application::Run()
{
	SGraphics::Init(EGfxApi::OpenGL);

	MainWindow.reset(SGraphics::CreateGfxWindow(ScreenWidth, ScreenHeight, "Physical Suika"));
	Engine::Init();

	SRenderer::Init();

	GGame = new SGame();
	GAssert(GGame);

	STimestep Step(TargetFPS, MaxFrametimeMs);

	const FColorRGB Background = { 250, 250, 250 };
	SGraphics::SetClearColor(Background);

	while (!bQuit)
	{
		Step.FrameStart();

		// TODO: Pass dT to logic!
		GGame->Tick();

		do  // Allow phisics to catch up
		{
			Engine::GetPhyScene().Tick(Step.GetStep());
		}
		while (Step.Update());

		SGraphics::Clear();
		// Rendering Pass
		Engine::GetGraphics().Tick(GGame->GetCamera());
		MainWindow->Tick();

		GGame->CullEntities();
	}

	delete GGame;
}
