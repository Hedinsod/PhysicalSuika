#pragma once

#include "Utility.h"


class IWindow;
class SGame;


class Application final
{
public:
	Application();
	~Application();

	void Run();
	void Quit() { bQuit = true; };

	IWindow& GetWindow() { return *MainWindow; }

private:
	std::unique_ptr<IWindow> MainWindow;

	// Root Actor
	//SGame* TheGame;

	Int32 ScreenWidth = 800;
	Int32 ScreenHeight = 600;

	bool bQuit = false;
};
extern Application* GApp;
