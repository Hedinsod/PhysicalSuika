#include "pch.h"

#include "Core/Application.h"
#include "Core/Timestep.h"
#include "Graphics/Graphics.h"
#include "Graphics/GfxWindow.h"
#include "Systems/Engine.h"
#include "Game/Game.h"


Application* GApp = nullptr;

Application::Application() = default;

// All systems should be initialized when GApp is already constructed
void Application::Init()
{
	SGraphics::Init(EGfxApi::OpenGL);

	TheWindow = SGraphics::CreateGfxWindow(TheSettings.WindowWidth, TheSettings.WindowHeight, "Physical Suika");
	GAssert(TheWindow);
	
	Engine::Init();

	TheGame = MakeScoped<SGame>();
	GAssert(TheGame);

	// Notify every system about initial window size
	OnResize(TheSettings.WindowWidth, TheSettings.WindowHeight);
}

Application::~Application()
{
	// Eventually not so smart pointer 
	TheGame.reset();

	Engine::Shutdown();

	TheWindow->Destroy();

	SGraphics::Shutdown();
}

void Application::Run()
{
	STimestep Step(TheSettings.TargetFPS, TheSettings.MaxFrametimeMs);

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
		TheWindow->Tick();

		// Delete unused
		TheGame->CullEntities();
	}
}

void Application::OnResize(int32_t InScreenWidth, int32_t InScreenHeight)
{
	OnResizeEvent.Broadcast(InScreenWidth, InScreenHeight);
}
