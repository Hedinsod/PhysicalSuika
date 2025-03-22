#pragma once

#include "Core/Utility.h"
#include "Systems/GeometryComponent.h"
#include <unordered_set>


class Entity;
class SGfxContext;

class SGraphics
{
public:
	SGraphics(SGfxContext* InContext)
		: Context(InContext)
	{
	}

	CGeometry* CreateGeometry(Entity* InOwner);
	void RemoveGeometry(CGeometry* Geo);

	void Tick();

private:
	std::unordered_set<CGeometry*> GeometryPool;
	SGfxContext* Context;
};
