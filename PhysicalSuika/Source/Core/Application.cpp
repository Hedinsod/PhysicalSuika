#include "pch.h"

#include "Core/Application.h"
#include "Timestep.h"
#include "Graphics/GfxWindow.h"
#include "Graphics/Graphics.h"
#include "Systems/Engine.h"
#include "Game/Game.h"

#include "Renderer/Renderer.h"


Application::Application()
{
	SGraphics::Init(EGfxApi::OpenGL);

	MainWindow.reset(SGraphics::CreateGfxWindow(ScreenWidth, ScreenHeight, "Physical Suika"));

	Engine::Init();
	SRenderer::Init();

	TheGame = new SGame();
	GAssert(TheGame);
}

Application::~Application()
{
	delete TheGame;

	Engine::Shutdown();

	MainWindow->Destroy();
	SGraphics::Shutdown();
}

void Application::Run()
{
	STimestep Step(TargetFPS, MaxFrametimeMs);

	const FColorRGB Background = { 250, 250, 250 };
	SGraphics::SetClearColor(Background);

	while (!bQuit)
	{
		Step.FrameStart();

		// Physics
		Engine::GetPhyScene().Tick(Step);

		// Render everything
		SGraphics::Clear();
		Engine::GetGraphics().Tick(TheGame->GetCamera());

		// Poll input and swap buffers
		MainWindow->Tick();

		// Game logic
		TheGame->Tick(Step.GetFullStep());

		// Delete unused
		TheGame->CullEntities();
	}
}
