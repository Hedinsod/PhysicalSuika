#include "pch.h"
#include "Fruit.h"
#include "Arbiter.h"
#include "Game.h"
#include "Systems/Engine.h"
#include "Renderer/Renderer.h"
#include "Graphics/Types.h"

struct FruitInstance
{
	FColorRGB Color;
	float Scale;

	uint32_t Ui;
	uint32_t Vi;

	// UVs
};

static std::array<FruitInstance, (int16_t)EFruitType::Count> FruitInstances =
	{
		FruitInstance(FColorRGB(220, 65, 80), 1.0f, 0, 2),     // Cherry
		FruitInstance(FColorRGB(215, 100, 60), 1.5f, 1, 2),    // Strawberry
		FruitInstance(FColorRGB(200, 115, 245), 2.2f, 2, 2),   // Grape
		FruitInstance(FColorRGB(240, 175, 75), 2.4f, 3, 2),    // Dekopon
		FruitInstance(FColorRGB(215, 145, 90), 3.0f, 0, 1),    // Orange
		FruitInstance(FColorRGB(230, 90, 90), 3.6f, 1, 1),     // Apple
		FruitInstance(FColorRGB(235, 220, 100), 3.8f, 2, 1),   // Pear
		FruitInstance(FColorRGB(230, 105, 205), 5.1f, 3, 1),   // Peach
		FruitInstance(FColorRGB(245, 220, 65), 6.1f, 0, 0),    // Pineapple
		FruitInstance(FColorRGB(170, 250, 85), 8.7f, 1, 0),    // Melon
		FruitInstance(FColorRGB(135, 205, 50), 8.9f, 2, 0)     // Watermelon
	};


AFruit::AFruit(glm::vec2 InPos, EFruitType InType)
	: AActor(InPos)
	, Type(InType)
{
	FruitInstance& Inst = FruitInstances[(int16_t)Type];
	Trans.SetScale({ Inst.Scale, Inst.Scale });

	// Geometry component
	GeoHandle = Engine::Renderer().CreateGeometry(this);
	GeoHandle->Import({
		glm::vec2(-0.35f, -0.35f),
		glm::vec2(0.35f, -0.35f),
		glm::vec2(0.35f, 0.35f),
		glm::vec2(-0.35f, 0.35f) });

	const float U = Inst.Ui * 0.25f;
	const float V = Inst.Vi * 0.33f;
	GeoHandle->SetUVs({
		glm::vec2(U, V),
		glm::vec2(U + 0.25f, V),
		glm::vec2(U + 0.25f, V + 0.33f),
		glm::vec2(U, V + 0.33f) });
	GeoHandle->SetIndices({ 0, 1, 2, 2, 3, 0 });
	
	GeoHandle->SetMaterial("Berry");

	// Physics
	FColliderShape* Shape = FColliderShape::Create<FCircleCollider>({ 0, 0 }, Inst.Scale * 0.35f);
	Box = Engine::GetPhyScene().CreateRigidBody(this, "Berry", Shape);

	Engine::GetPhyScene().GetRigidBody(Box).SetOnCollisionEventHandler(std::bind(&AFruit::OnCollision, this, std::placeholders::_1));
}

AFruit::~AFruit()
{
	Engine::GetPhyScene().RemoveRigidBody(Box);
	Engine::Renderer().RemoveGeometry(GeoHandle);
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
		// Other will be disabled by itself
		Engine::GetPhyScene().GetRigidBody(Box).Disable();

		// Spawn between frames!
		if (Engine::GetPhyScene().GetRigidBody(Other->Box).IsDisabled())
		{
			GetGame()->GetArbiter().AddTask(this, Other);
		}
	}
}
