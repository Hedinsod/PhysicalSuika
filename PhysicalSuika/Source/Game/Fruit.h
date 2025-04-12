#pragma once

#include "Actor.h"
#include "Physics/RigidBodyComp.h"

#include "Renderer/GeometryHandle.h"

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

	// "Safe" way to spawn new entities
	virtual void Tick(float DeltaTimeMs) override;

	// Events
	void OnCollision(AActor* Opponent);

	// Just enable and disable physics
	void Hold();
	void Release();

private:
	FGeometryHandle GeoHandle;
	CBodyHandle Box;

	EFruitType Type;

	bool bMother = false;
	glm::vec2 SpawnPoint;

};
