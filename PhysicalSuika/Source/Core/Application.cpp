#include "pch.h"

#include "Core/Application.h"
#include "Graphics/GfxWindow.h"
#include "Graphics/Graphics.h"
#include "Systems/Engine.h"
#include "Game/Game.h"


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
	MainWindow.reset(SGraphics::CreateGfxWindow(ScreenWidth, ScreenHeight, "Physical Suika"));
	Engine::Init();

	GGame = new SGame(ScreenWidth, ScreenHeight);
	GAssert(GGame);

	DWORD lastTime = timeGetTime();

	while (!bQuit)
	{
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

		// TODO: delete, window should deal with graphics
		Engine::GetGraphics().Tick();
		MainWindow->Tick();

		GGame->CullEntities();
	}

	delete GGame;
}
