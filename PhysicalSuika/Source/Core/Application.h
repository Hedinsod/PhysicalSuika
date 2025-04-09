#pragma once

#include "Utility.h"

class SGfxWindow;
class SGame;


class Application final
{
public:
	Application();
	~Application();

	void Run();
	void Quit() { bQuit = true; };

	SGfxWindow& GetWindow() { return *MainWindow; }

private:
	std::unique_ptr<SGfxWindow> MainWindow;

	// Scene - holds all actors of the game
	SGame* TheGame;

	bool bQuit = false;

	// Settings
	int32_t ScreenWidth = 800;
	int32_t ScreenHeight = 600;

	static constexpr float TargetFPS = 60.f;
	static constexpr float MaxFrametimeMs = 200.f;
};
extern Application* GApp;
