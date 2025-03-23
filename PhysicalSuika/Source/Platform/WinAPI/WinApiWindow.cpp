#include "pch.h"

#include "Core/Application.h"
#include "Core/Input.h"
#include "Systems/Draw.h"
#include "Platform/WinAPI/WinApiWindow.h"
#include "Platform/WinAPI/WinApiGraphics.h"

SWinApiWindow::SWinApiWindow(int InWidth, int InHeight, const std::string& InTitle)
	: SGfxWindow(InWidth, InHeight, InTitle)
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

static EInputCode GetInputCode(WPARAM wParam)
{
	switch (wParam)
	{
	case VK_LEFT:   return EInputCode::Left;  break;
	case VK_UP:     return EInputCode::Up;    break;
	case VK_RIGHT:  return EInputCode::Right; break;
	case VK_DOWN:   return EInputCode::Down;  break;
	case VK_SPACE:  return EInputCode::Space; break;
	case VK_ESCAPE: return EInputCode::Esc;   break;
	default:        return EInputCode::None;  break;
	};
}

LRESULT CALLBACK SWinApiWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE: GApp->Quit(); break;
	case WM_PAINT:
	{
		SGfxContext* Context = GApp->GetWindow().GetGfxContext();
		Context->FillRect({ 0, 0 }, { 800, 600 }, FColor{ 0,0,0 });
		break;
	}
	case WM_KEYDOWN: SInput::InputEvent(GetInputCode(wParam), true);  break;
	case WM_KEYUP:   SInput::InputEvent(GetInputCode(wParam), false);  break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}
