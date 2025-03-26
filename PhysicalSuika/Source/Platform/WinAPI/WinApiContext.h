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

	virtual void SwapBuffers() override {}

private:
	HWND hWnd;

};
