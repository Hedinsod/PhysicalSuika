#include "pch.h"

#include "Core/Application.h"
#include "Core/Input.h"
#include "Systems/Graphics.h"
#include "Platform/WinAPI/WinApiWindow.h"
#include "Platform/WinAPI/WinApiGraphics.h"


std::unique_ptr<IWindow> IWindow::Create(int InWidth, int InHeight, const std::string& InTitle)
{
	return std::make_unique<SWinApiWindow>(InWidth, InHeight, InTitle);
}

SWinApiWindow::SWinApiWindow(int InWidth, int InHeight, const std::string& InTitle)
	: IWindow(InWidth, InHeight, InTitle)
	, hWnd(NULL)
{
	Create();
}

void SWinApiWindow::Create()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

	WNDCLASSEX wc{ 0 };

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = SWinApiWindow::WndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszClassName = "MainWindow";
	RegisterClassEx(&wc);

	DWORD dw_style = WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN;
	RECT rc;
	SetRect(&rc, 0, 0, Width, Height);
	AdjustWindowRect(&rc, dw_style, false);
	rc.right -= rc.left;
	rc.bottom -= rc.top;

	hWnd = CreateWindowEx(0,
		"MainWindow",           //ClassName
		Title.c_str(),
		dw_style | WS_VISIBLE,
		(GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2,
		rc.right, rc.bottom,
		NULL,                   //hwndParent
		NULL,
		hInstance,
		NULL);                  //lpvParam

	GfxContext = new SWinApiContext(hWnd);
}

void SWinApiWindow::Destroy()
{
	DestroyWindow(hWnd);
	hWnd = NULL;
}

void SWinApiWindow::Tick()
{
	// Windows messaging
	MSG msg;
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			GApp->Quit();
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT CALLBACK SWinApiWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		break;
	}
	case WM_PAINT:
	{
		SGfxContext* Context = GApp->GetWindow().GetGfxContext();
		Context->FillRect({ 0, 0 }, { 800, 600 }, FColor{ 0,0,0 });
		break;
	}
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_LEFT:  SInput::InputEvent(EInputCode::Left, true);  break;
		case VK_UP:    SInput::InputEvent(EInputCode::Up, true);    break;
		case VK_RIGHT: SInput::InputEvent(EInputCode::Right, true); break;
		case VK_DOWN:  SInput::InputEvent(EInputCode::Down, true);  break;
		case VK_SPACE: SInput::InputEvent(EInputCode::Space, true); break;
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
	}
	case WM_KEYUP:
	{
		switch (wParam)
		{
		case VK_LEFT:  SInput::InputEvent(EInputCode::Left, false);  break;
		case VK_UP:    SInput::InputEvent(EInputCode::Up, false);    break;
		case VK_RIGHT: SInput::InputEvent(EInputCode::Right, false); break;
		case VK_DOWN:  SInput::InputEvent(EInputCode::Down, false);  break;
		case VK_SPACE: SInput::InputEvent(EInputCode::Space, false); break;
		}
		break;
	}
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}
