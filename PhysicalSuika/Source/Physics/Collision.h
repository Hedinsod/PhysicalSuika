#pragma once

#include "Systems/Components.h"
#include <unordered_set>

#include <glm/glm.hpp>

class AActor;

struct FBoundingBox
{
	glm::vec2 Max;
	glm::vec2 Min;
};

// Axis Aligned Bounding Box - AABB
class CPhysicsComp : public CComponent
{
public:
	CPhysicsComp(AActor* InOwner)
		: CComponent(InOwner)
	{
	}

	inline void SetupBoundingBox(float Left, float Top, float Right, float Bottom)
	{
		Box.Max.x = Right;
		Box.Max.y = Top;
		Box.Min.x = Left;
		Box.Min.y = Bottom;
	}

	inline const FBoundingBox& GetBoundingBox() { return Box; }

private:
	FBoundingBox Box;

};

class SCollisionCheck
{
public:
	CPhysicsComp* AddPhysics(AActor* Owner);
	void RemovePhysics(CPhysicsComp* Comp);

	void Tick();

private:
	bool SimpleCollision(const FBoundingBox& BoxA, const FBoundingBox& BoxB);

	std::unordered_set<CPhysicsComp*> PhComps;

};
