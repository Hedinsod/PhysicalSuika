#include "pch.h"
#include "Hand.h"
#include "Game.h"
#include "Fruit.h"
#include "Core/Input.h"
#include "Renderer/GeometryComp.h"
#include "Systems/Engine.h"


static const struct FHandData
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
	std::vector<uint32_t> Indices = {
		0,1,2, 1,2,3, 3,4,5, 5,6,7, 6,7,8
	};

	float HandSpeed = 0.1f;
	glm::vec2 FruitOffset{ 0.f, -1.5f };

	float MovementHalfLimit = 4.0f; // 80-90% of the glass half width (5.0)
	float SpawnCooldownMax = 1.0f; // 1 second

} HandData;




AHand::AHand(glm::vec2 InPos)
	: AActor(InPos)
	, SpawnCooldown(0.0f)
	, HandOffset(0.0f)
	, bPaused(false)
{
	GeoHandle = FGeometryHandle::Create(this);
	GeoHandle->SetVertices(HandData.Points);
	GeoHandle->SetIndices(HandData.Indices);
	GeoHandle->SetColor(FColorRGB(0, 0, 0));
}

AHand::~AHand()
{
	GeoHandle.Erase();
}

void AHand::Hold()
{
	bPaused = true;
}

void AHand::Release()
{
	bPaused = false;
}

void AHand::Tick(float DeltaTime)
{
	if (bPaused)
		return;

	// Hand movement
	if (SInput::IsButtonPressed(EInputCode::Left))
	{
		HandOffset -= HandData.HandSpeed;
	}
	if (SInput::IsButtonPressed(EInputCode::Right))
	{
		HandOffset += HandData.HandSpeed;
	}

	HandOffset = glm::clamp(HandOffset, -HandData.MovementHalfLimit, HandData.MovementHalfLimit);
	const glm::vec2 NewPosition = glm::vec2(HandOffset, Trans.GetPos().y);

	Trans.SetPos(NewPosition);
	glm::vec2 HoldPoint = NewPosition + HandData.FruitOffset;


	// New fruit (preview) spawn with cooldown
	if (!Preview)
	{
		if (SpawnCooldown > 0.0f)
		{
			SpawnCooldown -= DeltaTime;
		}
		else
		{
			SpawnPreview(HoldPoint);
		}
	}


	// Preview release by user
	if (Preview)
	{
		Preview->GetTransform().SetPos(HoldPoint);

		if (SInput::IsButtonPressed(EInputCode::Space))
		{
			Preview->Release();
			Preview.reset();

			SpawnCooldown = HandData.SpawnCooldownMax;
		}
	}
}

void AHand::SpawnPreview(const glm::vec2& InSpawnPoint)
{
	uint16_t TypeRand = Math::GetRandom((uint16_t)EFruitType::Cherry, (uint16_t)EFruitType::Apple);
	Preview = GetGame()->AddEntity<AFruit>(InSpawnPoint, (EFruitType)TypeRand);
	Preview->Hold();
}
