#include "pch.h"

#include "Systems/Collision.h"
#include "Game/Game.h"
#include "Game/Actor.h"
#include "Renderer/Transform.h"


CollisionBox CollisionBox::GetTransformedBox()
{
	return CollisionBox(Owner,
		Owner->GetTransform().GetPos().y + Top,
		Owner->GetTransform().GetPos().x + Left,
		Owner->GetTransform().GetPos().x + Right,
		Owner->GetTransform().GetPos().y + Bottom);
}

void SCollisionCheck::AddCollisionBox(CollisionBox* Box)
{
	Boxes.insert(Box);
}

void SCollisionCheck::RemoveCollisionBox(CollisionBox* Box)
{
	Boxes.erase(Box);
}

void SCollisionCheck::Tick()
{
	if (Boxes.empty())
		return;

	std::vector<CollisionBox> FrameCache;
	for (CollisionBox* Box : Boxes)
	{
		CollisionBox BoxT = Box->GetTransformedBox();

		bool bVertical = false;
		bool bHorizontal = false;
		if (BoxT.Right > GGame->Right || BoxT.Left < GGame->Left)
		{
			bHorizontal = true;
		}
		if (BoxT.Top > GGame->Top || BoxT.Bottom < GGame->Bottom)
		{
			bVertical = true;
		}
		if (bHorizontal || bVertical)
		{
			BoxT.Owner->OnOutOfBounds(bHorizontal, bVertical);
		}

		FrameCache.push_back(std::move(BoxT));
	}

	for (auto First = FrameCache.begin(); First != FrameCache.end() - 1; First++)
	{
		for (auto Second = First + 1; Second != FrameCache.end(); Second++)
		{
			if (Second == First)
			{
				continue;
			}

			float TargetWidth = Second->Right - Second->Left
				+ First->Right - First->Left;
			float FactWidth = Second->Right - First->Left;

			float TargetHeight = Second->Top - Second->Bottom
				+ First->Top - First->Bottom;
			float FactHeight = Second->Top - First->Bottom;

			if (FactWidth > 0 && FactWidth < TargetWidth
				&& FactHeight > 0 && FactHeight < TargetHeight)
			{
				First->Owner->OnCollide(Second->Owner);
				Second->Owner->OnCollide(First->Owner);
			}
		}
	}
}