#include "pch.h"

#include "Core/Input.h"
#include "Systems/Engine.h"
#include "Game/Game.h"
#include "Game/Cannon.h"
#include "Game/Bullet.h"


ECannon::ECannon(float InX, float InY)
	: Entity(InX, InY)
	, Angle(0.f)
{
	Geo = Engine::GetGraphics().CreateGeometry(this);
	Geo->AddPoint({ -Width, +10 }, { 0, 255, 0 });
	Geo->AddPoint({ 0, -Height }, { 0, 0, 255 });
	Geo->AddPoint({  Width, +10 }, { 255, 0, 0 });
	Geo->AddPoint({ -Width, +10 }, { 0, 0, 0 });
}

ECannon::~ECannon()
{
	Engine::GetGraphics().RemoveGeometry(Geo);
	delete Geo;
}

void ECannon::Tick()
{
	if (SInput::IsButtonPressed(EInputCode::Left))
	{
		Angle = std::fminf(1.5708f, Angle + AngleStep);
	}

	if (SInput::IsButtonPressed(EInputCode::Right))
	{
		Angle = std::fmaxf(-1.5708f, Angle - AngleStep);
	}

	Trans.SetRotation(-Angle);

	static int CooldownCounter = 0;
	if (SInput::IsButtonPressed(EInputCode::Space) && CooldownCounter == 0)
	{
		float SpawnX = Trans.Pos.x;
		float SpawnY = Trans.Pos.y;

		GGame->AddEntity<EBullet>(SpawnX, SpawnY, Angle);
		CooldownCounter = ShootingCooldown;
	}

	if (CooldownCounter > 0) { --CooldownCounter; }
}
