#include "pch.h"
#include "Hand.h"
#include "Systems/Engine.h"
#include "Core/Input.h"
#include "Game.h"
#include "Fruit.h"

AHand::AHand(float InX, float InY)
	: AActor(InX, InY)
{
	Geo = Engine::GetGraphics().CreateGeometry(this);

	FColor Color({ 200, 165, 100 });
	Geo->AddPoint({ -10, 15 }, Color);
	Geo->AddPoint({ -15, 7 }, Color);
	Geo->AddPoint({ -10, 0 }, Color);
	Geo->AddPoint({ -3, 0 }, Color);
	Geo->AddPoint({ 0, -3 }, Color);
	Geo->AddPoint({ +3, 0 }, Color);
	Geo->AddPoint({ 10, 0 }, Color);
	Geo->AddPoint({ 13, 7 }, Color);
	Geo->AddPoint({ 10, 15 }, Color);

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