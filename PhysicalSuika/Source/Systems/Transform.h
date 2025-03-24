#pragma once

#include "Graphics/Graphics.h"

class AActor;

struct CTransform
{
	CTransform(AActor* InOwner, float InX, float InY);

	void SetRotation(float Angle);

	FPoint Pos;
	FPoint Rot;

	AActor* Owner;
};