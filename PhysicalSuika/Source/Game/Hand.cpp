#include "pch.h"
#include "Hand.h"
#include "Game.h"
#include "Fruit.h"
#include "Core/Input.h"
#include "Renderer/GeometryComp.h"
#include "Systems/Engine.h"

AHand::AHand(float InX, float InY)
	: AActor(InX, InY)
{
	Geo = Engine::GetGraphics().CreateGeometry(this);

	FColor Color({ 200, 165, 100 });

	Geo->SetVertices({
		-10, 15,
		-15,  7,
		-10,  0,
		 -3,  0,
		  0, -3,
		  3,  0,
		 10,  0,
		 13,  7,
		 10, 15
		});
	Geo->SetIndecies({
		0, 1, 2, 3, 4, 5, 6, 7, 8
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
	static const FPoint Shift{ 3, 0 };
	if (SInput::IsButtonPressed(EInputCode::Left))
	{
		Trans.Pos -= Shift;
	}
	if (SInput::IsButtonPressed(EInputCode::Right))
	{
		Trans.Pos += Shift;
	}
	
	if (SpawnCooldown > 0) SpawnCooldown--;
	if (SInput::IsButtonPressed(EInputCode::Space) && SpawnCooldown == 0)
	{
		float SpawnX = Trans.Pos.x;
		float SpawnY = Trans.Pos.y + 15;

		GGame->AddEntity<AFruit>(SpawnX, SpawnY);
		SpawnCooldown = SpawnCooldownStart;
	}
}