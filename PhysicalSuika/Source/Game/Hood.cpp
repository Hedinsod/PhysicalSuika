#include "pch.h"
#include "Hood.h"
#include "Systems/Engine.h"
#include "Fruit.h"
#include "Game.h"
#include "Arbiter.h"

// Helper struct storing all raw data
static struct FHoodData
{
	// Top, Botton, Left, Right
	FRect Rect = FRect(0.5f, -0.5f, -5.5f, 5.5f);
	std::vector<uint32_t> Indices = { 0, 1, 3, 1, 2, 3 };
	glm::vec2 Pivot = glm::vec2(0.0f, 0.0f);

	std::string MaterialTag = "Default";
	FColorRGB Color = FColorRGB(255, 200, 15);
} HoodData;


AHood::AHood(glm::vec2 InPos)
	: AActor(InPos)
{
	GeoHandle = FGeometryHandle::Create(this);
	GeoHandle->SetVertices({
				glm::vec2(HoodData.Rect.Left, HoodData.Rect.Top),
				glm::vec2(HoodData.Rect.Left, HoodData.Rect.Bottom),
				glm::vec2(HoodData.Rect.Right, HoodData.Rect.Bottom),
				glm::vec2(HoodData.Rect.Right, HoodData.Rect.Top)
		});
	GeoHandle->SetIndices(HoodData.Indices);
	GeoHandle->SetMaterial(HoodData.MaterialTag);
	GeoHandle->SetColor(HoodData.Color);

	FColliderShape* TriggerBox = FColliderShape::Create<FBoxCollider>(HoodData.Pivot, HoodData.Rect.Left, HoodData.Rect.Top, HoodData.Rect.Right, HoodData.Rect.Bottom);
	Trigger = Engine::GetPhyScene().CreateRigidBody(this, HoodData.MaterialTag, TriggerBox, /* bTrigger */true);

	Engine::GetPhyScene().GetRigidBody(Trigger).SetOnCollisionEventHandler(std::bind(&AHood::OnCollision, this, std::placeholders::_1));
}

AHood::~AHood()
{
	Engine::GetPhyScene().RemoveRigidBody(Trigger);

	GeoHandle.Erase();
}

void AHood::OnCollision(AActor* Other)
{
	AFruit* Trespasser = dynamic_cast<AFruit*>(Other);
	if (Trespasser)
	{
		Trespasser->Risk();
	}
}
