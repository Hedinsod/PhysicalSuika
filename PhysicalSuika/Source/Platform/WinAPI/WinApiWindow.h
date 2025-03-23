#pragma once

#include "Graphics/GfxWindow.h"

#undef UNICODE
#include <windows.h>

class SWinApiWindow : public SGfxWindow
{
public:
	SWinApiWindow(int InWidth, int InHeight, const std::string& InTitle);

	void Create();
	virtual void Destroy() override;
	virtual void* GetNativeWindow() { return &hWnd; }

	virtual void Tick() override;

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	HWND hWnd;

};
