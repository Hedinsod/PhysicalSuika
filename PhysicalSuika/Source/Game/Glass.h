#pragma once

#include "Actor.h"
#include "Physics/RigidBodyComp.h"
#include "Renderer/GeometryHandls.h"


class CGeometry;

class AGlass : public AActor
{
public:
	AGlass(glm::vec2 InPos);
	virtual ~AGlass() override;

private:
	FGeometryHandle GeoHandle;

	std::vector<FPrimitiveObject> TitleLabel;
	std::vector<FPrimitiveObject> ScoreLabel;

	CBodyHandle Left;
	CBodyHandle Right;
	CBodyHandle Bottom;

};
