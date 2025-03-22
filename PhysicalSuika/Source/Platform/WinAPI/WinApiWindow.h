#pragma once

#include "Core/Window.h"

#undef UNICODE
#include <windows.h>


class SWinApiWindow : public IWindow
{
public:
	SWinApiWindow(int InWidth, int InHeight, const std::string& InTitle);

	virtual void Create();
	virtual void Destroy() override;
	virtual void* GetNativeWindow() { return &hWnd; }

	virtual void Tick() override;

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	HWND hWnd;

};
