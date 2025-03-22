#include "pch.h"
#include "Game/Bullet.h"
#include "Game/Game.h"
#include "Systems/Engine.h"


EBullet::EBullet(float InX, float InY, float InAngle)
	: Entity(InX, InY)
{
	Velocity.x = -Speed * std::sin(InAngle);
	Velocity.y = -Speed * std::cos(InAngle);

	Collider = new CollisionBox(this, 3, -3, 3, -3);
	Engine::GetCollision().AddCollisionBox(Collider);

	Geo = Engine::GetGraphics().CreateGeometry(this);
	Geo->AddPoint({ 0, 0 }, { 255, 0, 0 });
	Geo->AddPoint({ 0, 1 }, { 0, 0, 0 });
}

EBullet::~EBullet()
{
	Engine::GetCollision().RemoveCollisionBox(Collider);
	delete Collider;

	Engine::GetGraphics().RemoveGeometry(Geo);
	delete Geo;
}

void EBullet::OnCollide(const Entity* Opponent)
{
	bPendingDelete = true;
}

void EBullet::OnOutOfBounds(bool bHorizontal, bool bVertical)
{
	bPendingDelete = true;
}

void EBullet::Tick()
{
	Trans.Pos += Velocity;

	Geo->SetPoint(1, Velocity);
}
