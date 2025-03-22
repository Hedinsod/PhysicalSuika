#pragma once
#include "Entity.h"


struct CollisionBox;
struct CGeometry;
class EClock;

class EHand : public  Entity
{
public:
	EHand(EClock* Clock, float InX, float InY, float InLength);
	virtual ~EHand() override;

	void Tick();

	void SetProgress(float InProgress) { Progress = InProgress; };

private:
	EClock* Parent;
	float Length;
	float Progress;

	CGeometry* Hand;
};

class EClock : public Entity
{
public:
	EClock(float InX, float InY, float InScale);
	virtual ~EClock() override;

	void Tick();

	//virtual void OnCollide(const Entity* Opponent) override;
	//virtual void OnOutOfBounds(bool bHorizontal, bool bVertical) override;

private:
	static constexpr int BaseLength = 5;
	float Length;
	float Scale;

	CollisionBox* Collider;
	CGeometry* Box;

	EHand* HourHand;
	EHand* MinHand;
	EHand* SecHand;
};