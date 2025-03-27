#include "pch.h"

#include "Core/Application.h"
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

	DWORD lastTime = timeGetTime();

	SGraphics::SetClearColor({ 220, 030, 220 });

	while (!bQuit)
	{
		SGraphics::Clear();

		const DWORD kSleepTimeMs = 20;
		const DWORD currentTime = timeGetTime();
		const DWORD timeSinceLast = currentTime - lastTime;

		if (timeSinceLast < kSleepTimeMs)
		{
			Sleep(kSleepTimeMs - timeSinceLast);
		}

		lastTime = currentTime;

		GGame->Tick();
		Engine::GetCollision().Tick();

		Engine::GetGraphics().Tick(GGame->GetCamera());
		MainWindow->Tick();

		GGame->CullEntities();
	}

	delete GGame;
}
