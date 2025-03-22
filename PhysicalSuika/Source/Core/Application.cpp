#include "pch.h"

#include "Core/Application.h"
#include "Core/Window.h"
#include "Systems/Engine.h"
#include "Systems/Graphics.h"
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
	MainWindow = IWindow::Create(ScreenWidth, ScreenHeight, "Phisical Suika");
	Engine::Init(MainWindow->GetGfxContext());

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

		MainWindow->Tick();

		GGame->Tick();

		Engine::GetCollision().Tick();
		Engine::GetGraphics().Tick();

		GGame->CullEntities();
	}

	delete GGame;
}
