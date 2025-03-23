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

	// Root Actor
	//SGame* TheGame;

	int32_t ScreenWidth = 800;
	int32_t ScreenHeight = 600;

	bool bQuit = false;
};
extern Application* GApp;
