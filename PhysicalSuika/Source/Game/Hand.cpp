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
	Geo = Engine::GetGraphics().CreateGeometry(this);

	FColorRGB Color({ 200, 165, 100 });

	Geo->SetVertices({
		-1.0f, -1.5f,
		-1.3f, -0.7f,
		-1.0f,  0.0f,
		-0.3f,  0.0f,
		 0.0f,  0.3f,
		 0.3f,  0.0f,
		 1.0f,  0.0f,
		 1.3f, -0.7f,
		 1.0f, -1.5f
		});

	Geo->SetIndices({
		0, 1, 2, 3, 4, 5, 6, 7, 8
		});
	Geo->BuildGeometry();
	Geo->SetColor(FColorRGB(0, 0, 0));
}

AHand::~AHand()
{
	Engine::GetGraphics().RemoveGeometry(Geo);
}

void AHand::Tick(float DeltaTimeMs)
{
	static const glm::vec2 Shift{ .1f, 0.f };
	
	if (SInput::IsButtonPressed(EInputCode::Left))
	{
		Trans.Translate(-Shift);
	}
	if (SInput::IsButtonPressed(EInputCode::Right))
	{
		Trans.Translate(Shift);
	}
	glm::vec2 HoldPoint = Trans.GetPos() + glm::vec2{ 0.f, -1.5f };

	if (SpawnCooldown > 0.0f)
	{
		SpawnCooldown -= DeltaTimeMs;
	}

	if (!Preview && SpawnCooldown <= 0)
	{
		uint16_t TypeRand = Utility::GetRandom(1, 6);
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