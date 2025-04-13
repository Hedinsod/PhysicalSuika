#pragma once

#include "Utility.h"
#include "Settings.h"
#include "Event.h"

class SGfxWindow;
class SGame;


using FAppEvent_OnResize = TEvent<void, int32_t, int32_t>;

class Application final
{
public:
	Application();
	~Application();

	void Init();

	void Run();
	void Quit() { bQuit = true; };

	SGfxWindow& GetWindow() { return *TheWindow; }

	// Events
	void OnResize(int32_t InScreenWidth, int32_t InScreenHeight);

	void AddOnResizeEventHandler(const FAppEvent_OnResize::EventCallbackFn& InCallback)
	{
		OnResizeEvent.Subscribe(InCallback);
	}

private:
	// Main window of the application
	StdScoped<SGfxWindow> TheWindow;
	
	// Scene - holds all actors of the game
	StdScoped<SGame> TheGame;

	// Quit flag - breaks main loop of the app
	bool bQuit = false;

	// Events
	FAppEvent_OnResize OnResizeEvent;

	// Application Settings
	FSettings TheSettings;
};
extern Application* GApp;
