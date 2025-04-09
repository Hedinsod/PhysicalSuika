#pragma once

#include "Core/Utility.h"
#include <string>

class SGfxContext;

class SGfxWindow
{
public:
	SGfxWindow(int32_t InWidth, int32_t InHeight, const std::string& InTitle)
		: Title(InTitle)
		, Width(InWidth)
		, Height(InHeight)
	{
	}

	virtual void Destroy() = 0;
	virtual void Tick() {}

	virtual void* GetNativeWindow() = 0;
	SGfxContext* GetGfxContext() { return GfxContext; }

	// "Events"
	virtual void OnResize(int32_t InWidth, int32_t InHeight) = 0;

protected:
	std::string Title;
	int Width;
	int Height;

	SGfxContext* GfxContext = nullptr;

};
