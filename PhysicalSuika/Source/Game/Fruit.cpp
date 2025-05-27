#include "pch.h"
#include "Fruit.h"
#include "Arbiter.h"
#include "Game.h"
#include "Systems/Engine.h"
#include "Renderer/GeometryComp.h"
#include "Graphics/Types.h"

// Helper struct storing all raw data
static struct FFruitData
{
	glm::vec2 Pivot = glm::vec2(0.0f, 0.0f);

	// Top, Botton, Left, Right
	FRect Rect = FRect(1.0f, -1.0f, -1.0f, 1.0f);
	std::vector<uint32_t> Indices = { 0, 1, 2, 2, 3, 0 };
	float Scale = 0.35f;

	std::string MaterialTag = "Berry";

	const std::vector<glm::vec2> UvRect(uint32_t InUi, uint32_t InVi)
	{
		// sub-texture size in normalized UV-coordinates
		static const float UvWidth = 0.25f;
		static const float UvHeight = 0.33f;

		const float U = InUi * UvWidth;
		const float V = InVi * UvHeight;

		return { glm::vec2(U, V),
		         glm::vec2(U + UvWidth, V),
		         glm::vec2(U + UvWidth, V + UvHeight),
		         glm::vec2(U, V + UvHeight)
		};
	}
} FruitData;


struct FruitInstance
{
	FColorRGB Color;
	float Scale;

	// Index in tex atlas
	uint32_t Ui;
	uint32_t Vi;
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
	, bActivated(false)
	, Life(MaxLife) // number of seconds 
{
	GAssert(Type < EFruitType::Count);
	FruitInstance& Inst = FruitInstances[(int16_t)Type];
	Trans.SetScale({ Inst.Scale * FruitData.Scale, Inst.Scale * FruitData.Scale });

	// Geometry component
	GeoHandle = FGeometryHandle::Create(this);
	GeoHandle->SetVertices({
				glm::vec2(FruitData.Rect.Left, FruitData.Rect.Bottom),
				glm::vec2(FruitData.Rect.Right, FruitData.Rect.Bottom),
				glm::vec2(FruitData.Rect.Right, FruitData.Rect.Top),
				glm::vec2(FruitData.Rect.Left, FruitData.Rect.Top)
		});
	GeoHandle->SetUVs(FruitData.UvRect(Inst.Ui, Inst.Vi));
	GeoHandle->SetIndices(FruitData.Indices);
	GeoHandle->SetMaterial(FruitData.MaterialTag);

	// Physics
	FColliderShape* Shape = FColliderShape::Create<FCircleCollider>(FruitData.Pivot, Inst.Scale * FruitData.Scale);
	Box = Engine::GetPhyScene().CreateRigidBody(this, FruitData.MaterialTag, Shape);
}

AFruit::~AFruit()
{
	Engine::GetPhyScene().RemoveRigidBody(Box);
	GeoHandle.Erase();
}

void AFruit::Delete()
{
	Engine::GetPhyScene().GetRigidBody(Box).Disable();
	AActor::Delete();
}

void AFruit::Hold()
{
	Engine::GetPhyScene().GetRigidBody(Box).Disable();
}

void AFruit::Release()
{
	Engine::GetPhyScene().GetRigidBody(Box).Enable();
}

void AFruit::Risk()
{
	Life--;

	if (Life == 0)
	{
		GetGame()->GetArbiter().Finish();
	}
}
