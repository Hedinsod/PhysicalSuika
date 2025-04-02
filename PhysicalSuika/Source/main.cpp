#include "pch.h"
#include "Core/Application.h"


Application* GApp = nullptr;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	GApp = new Application;

	GApp->Run();

	delete GApp;

	return 0;
}
