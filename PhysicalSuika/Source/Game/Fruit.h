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

	virtual void Delete() override;

	// Just enable and disable physics
	void Hold();
	void Release();

	//
	void Risk();

	// Triggerable
	void Activate() { bActivated = true; }
	bool IsActivated() const { return bActivated; }

	EFruitType GetType() const { return Type; }

private:
	const int32_t MaxLife = 300; // number of frames the Fruit can leave outside of the glass

	FGeometryHandle GeoHandle;
	CBodyHandle Box;

	EFruitType Type;
	bool bActivated;

	int32_t Life;

};
