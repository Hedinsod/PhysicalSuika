#include "pch.h"

#include "Core/Application.h"
#include "Core/Timestep.h"
#include "Graphics/Graphics.h"
#include "Graphics/GfxWindow.h"
#include "Systems/Engine.h"
#include "Game/Game.h"


Application::Application()
	: TheGame(nullptr)
{
}

void Application::Init()
{
	SGraphics::Init(EGfxApi::OpenGL);

	MainWindow.reset(SGraphics::CreateGfxWindow(ScreenWidth, ScreenHeight, "Physical Suika"));

	Engine::Init();

	TheGame = new SGame();
	GAssert(TheGame);

	OnResize(ScreenWidth, ScreenHeight);
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

		// Game logic
		TheGame->Tick(Step.GetFullStep());

		// Render everything
		SGraphics::Clear();
		Engine::GetGraphics().Begin(TheGame->GetCamera());
		Engine::GetGraphics().Tick();
		Engine::GetGraphics().Finish();

		// Poll input and swap buffers
		MainWindow->Tick();

		// Delete unused
		TheGame->CullEntities();
	}
}

void Application::OnResize(int32_t InScreenWidth, int32_t InScreenHeight)
{
	OnResizeEvent.Broadcast(InScreenWidth, InScreenHeight);
}
