#pragma once

#include "Core/Utility.h"
#include "GfxContext.h"
#include <string>

class SGfxContext;

class SGfxWindow
{
public:
	SGfxWindow(uint32_t InWidth, uint32_t InHeight, const std::string& InTitle)
		: Title(InTitle)
		, Width(InWidth)
		, Height(InHeight)
	{
	}
	virtual ~SGfxWindow() = default;

	virtual void Destroy() = 0;
	virtual void Tick() {}

	SGfxContext& GetGfxContext() { return *GfxContext; }

	// Events
	virtual void OnResize(int32_t InWidth, int32_t InHeight) = 0;

protected:
	std::string Title;
	uint32_t Width;
	uint32_t Height;

	StdScoped<SGfxContext> GfxContext;

};
