#pragma once

#include "Graphics/Types.h"

class SGfxContext
{
public:
	virtual void FillRect(FPoint Coner1, FPoint Coner2, FColor Color) = 0;
	virtual void DrawLine(FPoint Start, FPoint End, FColor color) = 0;

private:

};
