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

	FColor Color({ 200, 165, 100 });

	Geo->SetVertices({
		-10, -15,
		-13,  -7,
		-10,   0,
		 -3,   0,
		  0,   3,
		  3,   0,
		 10,   0,
		 13,  -7,
		 10, -15
		});
	Geo->SetIndecies({
		0, 1, 2, 3, 4, 5, 6, 7, 8
		});
	Geo->SetColors({
		0.8f, 0.6f, 0.3f,
		0.8f, 0.6f, 0.3f,
		0.8f, 0.6f, 0.3f,
		0.8f, 0.6f, 0.3f,
		0.8f, 0.6f, 0.3f,
		0.8f, 0.6f, 0.3f,
		0.8f, 0.6f, 0.3f,
		0.8f, 0.6f, 0.3f,
		0.8f, 0.6f, 0.3f
		});
	Geo->BuildGeometry();

	//	Engine::GetCollision().AddCollisionBox()
}

AHand::~AHand()
{
	Engine::GetGraphics().RemoveGeometry(Geo);
}

void AHand::Tick()
{
	static const glm::vec2 Shift{ 1, 0 };
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
		glm::vec2 Spawn = Trans.GetPos() + glm::vec2{ 0.f, -15.f };

		GGame->AddEntity<AFruit>(Spawn);
		SpawnCooldown = SpawnCooldownStart;
	}
}