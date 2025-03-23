#include "pch.h"
#include "Input.h"
#include "Application.h"


bool SInput::InputData[(uint32_t)EInputCode::InputCode_Count] { false };

bool SInput::IsButtonPressed(EInputCode Code)
{
	return InputData[(uint32_t)Code];
}

void SInput::InputEvent(EInputCode Code, bool bStatus)
{
	if (Code == EInputCode::Esc)
	{
		GApp->Quit();
	}

	InputData[(uint32_t)Code] = bStatus;
}
