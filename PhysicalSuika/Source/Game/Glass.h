#pragma once

#include "Actor.h"
#include "Physics/RigidBodyComp.h"
#include "Renderer/GeometryHandle.h"


class CGeometry;

class AGlass : public AActor
{
public:
	AGlass(glm::vec2 InPos);
	virtual ~AGlass() override;

private:
	FGeometryHandle GeoHandle;

	CBodyHandle Left;
	CBodyHandle Right;
	CBodyHandle Bottom;

};
