#include "pch.h"
#include "Fruit.h"
#include "Game.h"
#include "Systems/Engine.h"
#include "Renderer/Renderer.h"
#include "Graphics/Types.h"


struct FruitInstance
{
	FColorRGB Color;
	float Scale;
	// UVs
};

static std::array<FruitInstance, (int16_t)EFruitType::Count> FruitInstances =
	{
		FruitInstance(FColorRGB(220, 65, 80), 1.0f),     // Cherry
		FruitInstance(FColorRGB(215, 100, 60), 1.5f),    // Strawberry
		FruitInstance(FColorRGB(200, 115, 245), 2.2f),   // Grape
		FruitInstance(FColorRGB(240, 175, 75), 2.4f),    // Dekopon
		FruitInstance(FColorRGB(215, 145, 90), 3.0f),    // Orange
		FruitInstance(FColorRGB(230, 90, 90), 3.6f),     // Apple
		FruitInstance(FColorRGB(235, 220, 100), 3.8f),   // Pear
		FruitInstance(FColorRGB(230, 105, 205), 5.1f),   // Peach
		FruitInstance(FColorRGB(245, 220, 65), 6.1f),    // Pineapple
		FruitInstance(FColorRGB(170, 250, 85), 8.7f),    // Melon
		FruitInstance(FColorRGB(135, 205, 50), 8.9f)    // Watermelon
	};


AFruit::AFruit(glm::vec2 InPos, EFruitType InType)
	: AActor(InPos)
	, Type(InType)
{
	float Scale = FruitInstances[(int16_t)Type].Scale;
	Trans.SetScale({ Scale, Scale });

	// Geometry component
	GeoHandle = Engine::GetGraphics().CreateGeometry(this);
	GeoHandle->Import({
		glm::vec2(-0.35f, -0.35f),
		glm::vec2(0.35f, -0.35f),
		glm::vec2(0.35f, 0.35f),
		glm::vec2(-0.35f, 0.35f) });
	GeoHandle->SetUVs({
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f) });
	GeoHandle->SetIndices({ 0, 1, 2, 2, 3, 0 });
	
	GeoHandle->SetMaterial("Berry");
	Trans.SetZOrder(0.2f);

	// Physics
	FColliderShape* Shape = FColliderShape::Create<FCircleCollider>({ 0, 0 }, Scale * 0.35f);
	Box = Engine::GetPhyScene().CreateRigidBody(this, "Berry", Shape);

	Engine::GetPhyScene().GetRigidBody(Box).SetOnCollisionEventHandler(std::bind(&AFruit::OnCollision, this, std::placeholders::_1));
}

AFruit::~AFruit()
{
	Engine::GetPhyScene().RemoveRigidBody(Box);
	Engine::GetGraphics().RemoveGeometry(GeoHandle);
}

void AFruit::Tick(float DeltaTime)
{
	// Theoretically spawning actor here is safe
	// Not necessarily - we'll see

	if (bMother)
	{
		StdShared<AFruit> NewFruit = GetGame()->AddEntity<AFruit>(SpawnPoint, (EFruitType)((int16_t)Type + 1));
	}
}

void AFruit::Hold()
{
	Engine::GetPhyScene().GetRigidBody(Box).Disable();
}

void AFruit::Release()
{
	Engine::GetPhyScene().GetRigidBody(Box).Enable();
}

void AFruit::OnCollision(AActor* Opponent)
{
	AFruit* Other = dynamic_cast<AFruit*>(Opponent);

	if (Other && Other->Type == Type && Type != EFruitType::Watermelon)
	{
		bPendingDelete = true;

		// Other will be disabled by itself
		Engine::GetPhyScene().GetRigidBody(Box).Disable();

		// Spawn between frames!
		if (Engine::GetPhyScene().GetRigidBody(Other->Box).IsDisabled())
		{
			bMother = true;
			SpawnPoint = (Trans.GetPos() + Other->Trans.GetPos()) * 0.5f;
		}
	}
}
