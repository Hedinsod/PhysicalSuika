#pragma once

#include "Utility.h"
#include "Event.h"

class SGfxWindow;
class SGame;


EVENT_TwoParam(FAppEvent_OnResize, int32_t /*Width*/, int32_t /*Height*/);

class Application final
{
public:
	Application();
	~Application();

	void Init();

	void Run();
	void Quit() { bQuit = true; };

	SGfxWindow& GetWindow() { return *MainWindow; }

	// Events
	void OnResize(int32_t InScreenWidth, int32_t InScreenHeight);

	void AddOnResizeEventHandler(const FAppEvent_OnResize::EventCallbackFn& InCallback)
	{
		OnResizeEvent.Subscribe(InCallback);
	}

private:
	StdScoped<SGfxWindow> MainWindow;

	// Scene - holds all actors of the game
	SGame* TheGame;

	bool bQuit = false;

	// Settings
	int32_t ScreenWidth = 800;
	int32_t ScreenHeight = 600;

	FAppEvent_OnResize OnResizeEvent;

	static constexpr float TargetFPS = 60.f;
	static constexpr float MaxFrametimeMs = 200.f;
};
extern Application* GApp;
