#include "pch.h"
#include "WinApiInput.h"


SInput* SInput::Instance = new SWinApiInput;

void SWinApiInput::InputEvent_Impl(EInputCode Code, bool bStatus)
{
	InputData[(UInt32)Code] = bStatus;
}

bool SWinApiInput::IsButtonPressed_Impl(EInputCode Code)
{
	return InputData[(UInt32)Code];
}
