#pragma once

#include "Utility.h"

enum class EInputCode : UInt32
{
	Left = 0,
	Up,
	Right,
	Down,
	Space,

	InputCode_Count
};

class SInput
{
public:
	static bool IsButtonPressed(EInputCode Code)
	{
		return Instance->IsButtonPressed_Impl(Code);
	}

	static void InputEvent(EInputCode Code, bool bStatus)
	{
		Instance->InputEvent_Impl(Code, bStatus);
	}

protected:
	virtual void InputEvent_Impl(EInputCode Code, bool bStatus) = 0;
	virtual bool IsButtonPressed_Impl(EInputCode Code) = 0;

private:
	static SInput* Instance;

};