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

	GeoHandle = Engine::GetGraphics().CreateGeometry(this);
	(*GeoHandle).Import(Points);

}

AHand::~AHand()
{
	Engine::GetGraphics().RemoveGeometry(GeoHandle);
}

void AHand::Tick(float DeltaTime)
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
		SpawnCooldown -= DeltaTime;
	}

	if (!Preview && SpawnCooldown <= 0)
	{
		uint16_t TypeRand = Math::GetRandom(1, 6);
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