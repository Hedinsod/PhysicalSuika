#include "pch.h"
#include "Hand.h"
#include "Game.h"
#include "Fruit.h"
#include "Core/Input.h"
#include "Renderer/GeometryComp.h"
#include "Systems/Engine.h"

AHand::AHand(glm::vec2 InPos)
	: AActor(InPos)
{
	std::vector<glm::vec2> Points = {
			glm::vec2(-1.0f, -1.5f),
			glm::vec2(-1.3f, -0.7f),
			glm::vec2(-1.0f,  0.0f),
			glm::vec2(-0.3f,  0.0f),
			glm::vec2(0.0f,  0.3f),
			glm::vec2(0.3f,  0.0f),
			glm::vec2(1.0f,  0.0f),
			glm::vec2(1.3f, -0.7f),
			glm::vec2(1.0f, -1.5f)
	};

	GeoHandle = FGeometryHandle::Create(this);
	GeoHandle->SetVertices(Points);
	GeoHandle->SetIndices({0,1,2, 1,2,3, 3,4,5, 5,6,7, 6,7,8});
	GeoHandle->SetColor(FColorRGB(0, 0, 0));
}

AHand::~AHand()
{
	GeoHandle.Erase();
}

void AHand::Tick(float DeltaTime)
{
	if (SInput::IsButtonPressed(EInputCode::Left))
	{
		HandOffset -= HandSpeed;
	}
	if (SInput::IsButtonPressed(EInputCode::Right))
	{
		HandOffset += HandSpeed;
	}

	// TODO: Do something with size of glass
	HandOffset = glm::clamp(HandOffset, -4.0f, 4.0f);
	Trans.SetPos({ HandOffset, Trans.GetPos().y });
	glm::vec2 HoldPoint = Trans.GetPos() + FruitOffset;

	if (SpawnCooldown > 0.0f)
	{
		SpawnCooldown -= DeltaTime;
	}

	if (!Preview && SpawnCooldown <= 0)
	{
		uint16_t TypeRand = Math::GetRandom((uint16_t)EFruitType::Cherry, (uint16_t)EFruitType::Apple);
		Preview = GetGame()->AddEntity<AFruit>(HoldPoint, (EFruitType)TypeRand);
		Preview->Hold();
	}

	if (Preview)
	{
		Preview->GetTransform().SetPos(HoldPoint);

		if (SInput::IsButtonPressed(EInputCode::Space))
		{
			Preview->Release();
			Preview.reset();

			SpawnCooldown = SpawnCooldownStart;
		}
	}
}