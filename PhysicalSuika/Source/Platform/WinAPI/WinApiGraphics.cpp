#include "pch.h"

#include "Core/Application.h"
#include "Platform/WinAPI/WinApiGraphics.h"
#include "Platform/WinAPI/WinApiWindow.h"


static inline unsigned int GetRGB(FColor C)
{
	return C.Red + (C.Green << 8) + (C.Blue << 16);
}

void SWinApiContext::FillRect(FPoint Pivot, FPoint Size, FColor Color)
{
	HDC hDC = GetDC(hWnd);
	HBRUSH Brush = CreateSolidBrush(GetRGB(Color));
	RECT Rect;

	const int PivotX = static_cast<int>(Pivot.x);
	const int PivotY = static_cast<int>(Pivot.y);
	const int EndX = static_cast<int>(Pivot.x + Size.x);
	const int EndY = static_cast<int>(Pivot.y + Size.y);

	SetRect(&Rect, PivotX, PivotY, EndX, EndY);
	::FillRect(hDC, &Rect, Brush);
	DeleteObject(Brush);
	ReleaseDC(hWnd, hDC);
}

void SWinApiContext::DrawLine(FPoint Start, FPoint End, FColor Color)
{
	HDC hDC = GetDC(hWnd);
	HPEN Pen = CreatePen(PS_SOLID, 1, GetRGB(Color));
	HGDIOBJ OldPen = SelectObject(hDC, Pen);

	const int StartX = static_cast<int>(Start.x);
	const int StartY = static_cast<int>(Start.y);
	const int EndX = static_cast<int>(End.x);
	const int EndY = static_cast<int>(End.y);

	MoveToEx(hDC, StartX, StartY, NULL);
	LineTo(hDC, EndX, EndY);

	SelectObject(hDC, OldPen);
	DeleteObject(Pen);
	ReleaseDC(hWnd, hDC);
}
