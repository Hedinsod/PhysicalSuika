#include "pch.h"
#include "Core/Application.h"


Application* GApp = nullptr;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	GApp = new Application;

	GApp->Run();

	delete GApp;

	return 0;
}
