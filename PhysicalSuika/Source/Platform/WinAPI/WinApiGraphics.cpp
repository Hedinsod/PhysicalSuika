#include "pch.h"
#include "WinApiGraphics.h"
#include "WinApiWindow.h"


SGfxWindow* SWinApiGraphics::CreateGfxWindow(int InWidth, int InHeight, const std::string& InTitle)
{
    return new SWinApiWindow(InWidth, InHeight, InTitle);
}

SGfxShaderPtr SWinApiGraphics::CreateShader(const std::string& VertexSource, const std::string& FragmentSource)
{
	// TODO: Needs deep facke
	return nullptr;
}

SGfxVertexBufferPtr SWinApiGraphics::CreateVertexBuffer(const std::vector<float>& VertexData)
{
	GAssert(false);
	return nullptr;
}

SGfxIndexBufferPtr SWinApiGraphics::CreateIndexBuffer(const std::vector<uint32_t>& IndexData)
{
	GAssert(false);
	return nullptr;
}

// Fake array ??
SGfxVertexDataPtr SWinApiGraphics::CreateVertexData()
{
	return nullptr;
}

// Main "render" ??
void SWinApiGraphics::DrawIndexed(const SGfxVertexDataPtr& VA)
{

}


// Some actuall code
static inline unsigned int GetRGB(FColor C)
{
	return C.Red + (C.Green << 8) + (C.Blue << 16);
}

void SWinApiGraphics::FillRect(FPoint Pivot, FPoint Size, FColor Color)
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

void SWinApiGraphics::DrawLine(FPoint Start, FPoint End, FColor Color)
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