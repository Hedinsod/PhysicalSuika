#include "pch.h"
#include "Hand.h"
#include "Game.h"
#include "Fruit.h"
#include "Core/Input.h"
#include "Renderer/GeometryComp.h"
#include "Systems/Engine.h"

AHand::AHand(glm::vec2 InPos)
	: AActor(InPos, "Hand\n")
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

	Geo->SetIndecies({
		0, 1, 2, 3, 4, 5, 6, 7, 8
		});
	Geo->BuildGeometry();
}

AHand::~AHand()
{
	Engine::GetGraphics().RemoveGeometry(Geo);
}

void AHand::Tick()
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
	
	if (SpawnCooldown > 0) SpawnCooldown--;
	if (SInput::IsButtonPressed(EInputCode::Space) && SpawnCooldown == 0)
	{
		glm::vec2 Spawn = Trans.GetPos() + glm::vec2{ 0.f, -1.5f };

		GGame->AddEntity<AFruit>(Spawn);
		SpawnCooldown = SpawnCooldownStart;
	}
}