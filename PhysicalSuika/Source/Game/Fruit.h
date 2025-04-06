#pragma once

#include "Actor.h"
#include "Physics/RigidBodyComp.h"

class CGeometry;

enum class EFruitType : int16_t
{
	Cherry = 1,
	Strawberry,
	Grape,
	Dekopon,
	Orange,
	Apple,
	Pear,
	Peach,
	Pineapple,
	Melon,
	Watermelon
};

class AFruit : public AActor
{
public:
	AFruit(glm::vec2 InPos, EFruitType InType);
	virtual ~AFruit() override;

	void Tick();

	void Disappear(AActor* Opponent);

private:
	CGeometry* Geo;
	CBodyHandle Box;

	EFruitType Type;

	static bool bMatCreated;
	static uint32_t MatId;
};
