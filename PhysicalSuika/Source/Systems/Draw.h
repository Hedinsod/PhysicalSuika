#pragma once

#include "Core/Utility.h"
#include <unordered_set>


class AActor;
class SGfxContext;
class CGeometry;
class ACamera;

class SDraw
{
public:
	SDraw(SGfxContext* InContext)
		: Context(InContext)
	{
	}

	CGeometry* CreateGeometry(AActor* InOwner);
	void RemoveGeometry(CGeometry* Geo);

	void Tick(const std::shared_ptr<ACamera>& Camera);

private:
	std::unordered_set<CGeometry*> GeometryPool;
	SGfxContext* Context;
};
