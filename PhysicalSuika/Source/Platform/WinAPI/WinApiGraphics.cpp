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

	const FPoint End = Pivot + Size;
	SetRect(&Rect, Pivot.x, Pivot.y, End.x, End.y);
	::FillRect(hDC, &Rect, Brush);
	DeleteObject(Brush);
	ReleaseDC(hWnd, hDC);
}

void SWinApiContext::DrawLine(FPoint Start, FPoint End, FColor Color)
{
	HDC hDC = GetDC(hWnd);
	HPEN Pen = CreatePen(PS_SOLID, 1, GetRGB(Color));
	HGDIOBJ OldPen = SelectObject(hDC, Pen);

	MoveToEx(hDC, Start.x, Start.y, NULL);
	LineTo(hDC, End.x, End.y);

	SelectObject(hDC, OldPen);
	DeleteObject(Pen);
	ReleaseDC(hWnd, hDC);
}
