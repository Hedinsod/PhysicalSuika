#include "pch.h"

#include "Collision.h"
#include "Game/Game.h"
#include "Game/Actor.h"
#include "Renderer/Transform.h"


CPhysicsComp* SCollisionCheck::AddPhysics(AActor* Owner)
{
	auto [It, bSuccess] = PhComps.emplace(new CPhysicsComp(Owner));
	GAssert(bSuccess);
	return *It;
}

void SCollisionCheck::RemovePhysics(CPhysicsComp* Comp)
{
	PhComps.erase(Comp);
	delete Comp;
}

bool SCollisionCheck::SimpleCollision(const FBoundingBox& BoxA, const FBoundingBox& BoxB)
{
	// Exit with no intersection if found separated along an axis 
	if (BoxA.Max.x < BoxB.Min.x || BoxA.Min.x > BoxB.Max.x) return false;
	if (BoxA.Max.y < BoxB.Min.y || BoxA.Min.y > BoxB.Max.y) return false;

	// No separating axis found, therefor there is at least one overlapping axis 
	return true;
}

void SCollisionCheck::Tick()
{
	if (PhComps.empty())
		return;

	std::vector<FBoundingBox> BBCache;
	std::vector<AActor*> Owners;
	int i = 0;
	for (CPhysicsComp* Comp : PhComps)
	{
		glm::vec2 Pos = Comp->GetOwner().GetTransform().GetPos();

		Owners.push_back(&Comp->GetOwner());
		BBCache.push_back(Comp->GetBoundingBox());
		BBCache[i].Min += Pos;
		BBCache[i].Max += Pos;

		i++;
	}

	for (int First = 0; First < BBCache.size() - 1; First++)
	{
		for (int Second = First + 1; Second < BBCache.size(); Second++)
		{
			if (SimpleCollision(BBCache[First], BBCache[Second]))
			{
				// Set position to touch?
				Owners[First]->OnCollide(*Owners[Second]);
				Owners[Second]->OnCollide(*Owners[First]);
			}
		}
	}
}