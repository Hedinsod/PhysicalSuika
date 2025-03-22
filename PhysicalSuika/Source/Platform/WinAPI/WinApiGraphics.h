#pragma once

#include "Graphics/GfxContext.h"
#include "windef.h"


class SWinApiContext : public SGfxContext
{
public:
	SWinApiContext(HWND InHWnd)
		: hWnd(InHWnd)
	{
	}

	virtual void FillRect(FPoint Pivot, FPoint Size, FColor Color) override;
	virtual void DrawLine(FPoint Start, FPoint End, FColor Color) override;

private:
	HWND hWnd;

};