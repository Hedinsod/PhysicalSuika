#pragma once

#include <unordered_set>

class Entity;

struct CollisionBox
{
	CollisionBox(Entity* InOwner, float InTop, float InLeft, float InRight, float InBottom)
		: Top(InTop)
		, Left(InLeft)
		, Right(InRight)
		, Bottom(InBottom)
		, Owner(InOwner)
	{
	}

	CollisionBox GetTransformedBox();

	float Top;
	float Left;
	float Right;
	float Bottom;

	Entity* Owner;
};

class SCollisionCheck
{
public:
	void AddCollisionBox(CollisionBox* Box);
	void RemoveCollisionBox(CollisionBox* Box);

	void Tick();

private:
	std::unordered_set<CollisionBox*> Boxes;

};
