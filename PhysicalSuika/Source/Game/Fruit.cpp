#include "pch.h"
#include "Fruit.h"
#include "Game.h"
#include "Systems/Engine.h"
#include "Renderer/GeometryPool.h"
#include "Graphics/Types.h"


static std::array<FColorRGB, (int16_t)EFruitType::Count> FruitColors =
	{
		FColorRGB(220, 65, 80),     // Cherry
		FColorRGB(215, 100, 60),    // Strawberry
		FColorRGB(200, 115, 245),   // Grape
		FColorRGB(240, 175, 75),    // Dekopon
		FColorRGB(215, 145, 90),    // Orange
		FColorRGB(230, 90, 90),     // Apple
		FColorRGB(235, 220, 100),   // Pear
		FColorRGB(230, 105, 205),   // Peach
		FColorRGB(245, 220, 65),    // Pineapple
		FColorRGB(170, 250, 85),    // Melon
		FColorRGB(135, 205, 50),    // Watermelon
	};


AFruit::AFruit(glm::vec2 InPos, EFruitType InType)
	: AActor(InPos)
	, Type(InType)
{
	float Scale = 0.4f + 0.15f * ((int16_t)Type + 1);
	Trans.SetScale({ Scale, Scale });

	// Generate points
	std::vector<glm::vec2> Points;
	Points.emplace_back(0.0f, 0.0f);
	uint32_t i = 0;
	for (float a = 0.f; a <= 360.f; a += 30.f)
	{
		Points.emplace_back(glm::sin(glm::radians(a)), glm::cos(glm::radians(a)));
	}

	// Geometry component
	GeoHandle = Engine::GetGraphics().CreateGeometry(this);
	GeoHandle->Import(Points);
	GeoHandle->SetMaterial("Berry");

	// Physics
	FColliderShape* Shape = FColliderShape::Create<FCircleCollider>({ 0, 0 }, Scale);
	Box = Engine::GetPhyScene().CreateRigidBody(this, "Berry", Shape);

	Engine::GetPhyScene().GetRigidBody(Box).SetOnCollisionEventHandler(std::bind(&AFruit::OnCollision, this, std::placeholders::_1));
}

AFruit::~AFruit()
{
	Engine::GetPhyScene().RemoveRigidBody(Box);
	Engine::GetGraphics().RemoveGeometry(GeoHandle);
}

void AFruit::Tick(float DeltaTimeMs)
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
