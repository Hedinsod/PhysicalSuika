#pragma once

#include "Core/Utility.h"
#include "Systems/GeometryComponent.h"
#include <unordered_set>


class AActor;
class SGfxContext;

class SDraw
{
public:
	SDraw(SGfxContext* InContext)
		: Context(InContext)
	{
	}

	CGeometry* CreateGeometry(AActor* InOwner);
	void RemoveGeometry(CGeometry* Geo);

	void Tick();

private:
	std::unordered_set<CGeometry*> GeometryPool;
	SGfxContext* Context;
};
