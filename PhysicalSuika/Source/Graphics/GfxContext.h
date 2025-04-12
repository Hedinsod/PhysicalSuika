#pragma once


class SGfxContext
{
public:
	virtual void SwapBuffers() = 0;

	// "Events"
	virtual void OnResize(int32_t InWidth, int32_t InHeight) = 0;

private:

};
