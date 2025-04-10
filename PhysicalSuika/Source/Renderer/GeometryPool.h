#pragma once

#include "Core/Utility.h"
#include <unordered_set>


class AActor;
class SGfxContext;
class CGeometry;
class ACamera;

class SGeometryPool
{
public:
	CGeometry* CreateGeometry(AActor* InOwner);
	void RemoveGeometry(CGeometry* Geo);

	void Tick(const StdShared<ACamera>& Camera);

private:
	std::unordered_set<CGeometry*> GeometryPool;

};
