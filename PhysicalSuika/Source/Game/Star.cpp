#include "pch.h"

#include "Systems/Engine.h"
#include "Game/Game.h"
#include "Game/Star.h"
#include "Game/Clock.h"


EStar::EStar(float InX, float InY, float InScale)
	: Entity(InX, InY)
	, Scale(InScale)
	, Angle(0.0f)
{
	int xVelocity = Utility::GetRandom(-6, 6);
	int yVelocity = Utility::GetRandom(-6, 6);

	Velocity = { (float)xVelocity, (float)yVelocity };

	float Side = BaseSide * Scale;

	Collider = new CollisionBox(this, +Side, -Side, +Side, -Side);
	Engine::GetCollision().AddCollisionBox(Collider);

	Box = Engine::GetGraphics().CreateGeometry(this);

	FColor BoxColor{ 0, 128, 128 };
	Box->AddPoint({ -Side, -Side }, BoxColor);
	Box->AddPoint({ -Side,  Side }, BoxColor);
	Box->AddPoint({ Side,  Side }, BoxColor);
	Box->AddPoint({ Side, -Side }, BoxColor);
	Box->AddPoint({ -Side, -Side }, BoxColor);  // for cycling

	int i = 0;
	FColor RayColor{ 0,0,200 };
	Rays[i] = Engine::GetGraphics().CreateGeometry(this);
	Rays[i]->AddPoint({ 0, -Side }, RayColor);
	Rays[i]->AddPoint({ 0, Side }, RayColor);

	Rays[++i] = Engine::GetGraphics().CreateGeometry(this);
	Rays[i]->AddPoint({ -Side, 0 }, RayColor);
	Rays[i]->AddPoint({ Side, 0 }, RayColor);

	float Diag = 0.707f * Side;
	Rays[++i] = Engine::GetGraphics().CreateGeometry(this);
	Rays[i]->AddPoint({ -Diag, -Diag }, RayColor);
	Rays[i]->AddPoint({ Diag, Diag }, RayColor);
	
	Rays[++i] = Engine::GetGraphics().CreateGeometry(this);
	Rays[i]->AddPoint({ -Diag, Diag }, RayColor);
	Rays[i]->AddPoint({ Diag, -Diag }, RayColor);
}

EStar::~EStar()
{
	Engine::GetCollision().RemoveCollisionBox(Collider);
	delete Collider;

	Engine::GetGraphics().RemoveGeometry(Box);
	delete Box;

	for (int i = 0; i < 4; i++)
	{
		Engine::GetGraphics().RemoveGeometry(Rays[i]);
		delete Rays[i];
	}
}

void EStar::Tick()
{
	Trans.Pos += Velocity;

	Angle += 0.1f;
	Trans.SetRotation(Angle);
}

void EStar::OnCollide(const Entity* Opponent)
{
	if (dynamic_cast<const EStar*>(Opponent) 
		|| dynamic_cast<const EClock*>(Opponent))
	{
		Velocity *= -1;
	}
	else // bullet
	{
		bPendingDelete = true;

		float Side = BaseSide * Scale;
		float NewScale = Scale / 2;
		if (NewScale > 1.0f)
		{
			GGame->AddEntity<EStar>(Trans.Pos.x - Side / 2, Trans.Pos.y + Side / 2, NewScale);
			GGame->AddEntity<EStar>(Trans.Pos.x + Side / 2, Trans.Pos.y - Side / 2, NewScale);
		}
	}
}

void EStar::OnOutOfBounds(bool bHorizontal, bool bVertical)
{
	if (bHorizontal)
	{
		Velocity.x *= -1;
	}
	if (bVertical)
	{
		Velocity.y *= -1;
	}
}