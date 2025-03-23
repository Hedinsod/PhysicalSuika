#pragma once

#include "Utility.h"

enum class EInputCode : uint32_t
{
	None = 0,
	Left,
	Up,
	Right,
	Down,
	Space,
	Esc,

	InputCode_Count
};

class SInput
{
public:
	static bool IsButtonPressed(EInputCode Code);
	static void InputEvent(EInputCode Code, bool bStatus);

private:
	static bool InputData[(uint32_t)EInputCode::InputCode_Count];

};