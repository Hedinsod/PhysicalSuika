#pragma once

#include <string>

class SGfxContext;

class IWindow
{
public:
	IWindow(int InWidth, int InHeight, const std::string& InTitle)
		: Title(InTitle)
		, Width(InWidth)
		, Height(InHeight)
	{
	}

	static std::unique_ptr<IWindow> Create(int InWidth, int InHeight, const std::string& InTitle);
	virtual void Destroy() = 0;
	virtual void* GetNativeWindow() = 0;
	SGfxContext* GetGfxContext() { return GfxContext; }

	virtual void Tick() {}

protected:
	std::string Title;
	int Width;
	int Height;

	SGfxContext* GfxContext = nullptr;

};
