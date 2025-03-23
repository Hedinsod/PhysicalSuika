#pragma once

#include "Graphics/Graphics.h"

class Entity;

struct CTransform
{
	CTransform(Entity* InOwner, float InX, float InY);

	void SetRotation(float Angle);

	FPoint Pos;
	FPoint Rot;

	Entity* Owner;
};