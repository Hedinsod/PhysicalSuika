#pragma once

#include "Actor.h"
#include "Physics/RigidBodyComp.h"
#include "Renderer/GeometryHandls.h"

class CGeometry;

enum class EFruitType : int16_t
{
	Cherry = 0,
	Strawberry,
	Grape,
	Dekopon,
	Orange,
	Apple,
	Pear,
	Peach,
	Pineapple,
	Melon,
	Watermelon,

	Count
};

class AFruit : public AActor
{
public:
	AFruit(glm::vec2 InPos, EFruitType InType);
	virtual ~AFruit() override;

	// Events
	void OnCollision(AActor* Opponent);

	// Just enable and disable physics
	void Hold();
	void Release();

	EFruitType GetType() const { return Type; }

private:
	FGeometryHandle GeoHandle;
	CBodyHandle Box;

	EFruitType Type;

};
