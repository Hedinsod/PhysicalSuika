#pragma once

#include "Graphics/Types.h"

class Entity;

struct Transform
{
	Transform(Entity* InOwner, float InX, float InY);

	void SetRotation(float Angle);

	FPoint Pos;
	FPoint Rot;

	Entity* Owner;
};