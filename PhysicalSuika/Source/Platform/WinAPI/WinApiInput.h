#pragma once

#include "Core/Input.h"

class SWinApiInput : public SInput
{
protected:
	virtual void InputEvent_Impl(EInputCode Code, bool bStatus) override;
	virtual bool IsButtonPressed_Impl(EInputCode Code) override;

private:
	bool InputData[(UInt32)EInputCode::InputCode_Count]{ false };
};