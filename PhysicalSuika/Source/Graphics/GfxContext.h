#pragma once

#include "Graphics/Graphics.h"

class SGfxContext
{
public:
	virtual void SwapBuffers() = 0;

	virtual void FillRect(FPoint Pivot, FPoint Size, FColor Color) = 0;
	virtual void DrawLine(FPoint Start, FPoint End, FColor color) = 0;

private:

};
