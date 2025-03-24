#include "pch.h"
#include "Fruit.h"
#include "Systems/Engine.h"

AFruit::AFruit(float InX, float InY)
	: AActor(InX, InY)
{
	Geo = Engine::GetGraphics().CreateGeometry(this);

	FColor Color({ 250, 5, 5 });
	Geo->AddPoint({  -5, +10 }, Color);
	Geo->AddPoint({ -10,   0 }, Color);
	Geo->AddPoint({  -5, -10 }, Color);
	Geo->AddPoint({   5, -10 }, Color);
	Geo->AddPoint({  10,   0 }, Color);
	Geo->AddPoint({   5, +10 }, Color);
	Geo->AddPoint({  -5, +10 }, Color);

	Box = new CollisionBox(this, 10, -10, 10, -10);
	Engine::GetCollision().AddCollisionBox(Box);
}

AFruit::~AFruit()
{
	Engine::GetCollision().RemoveCollisionBox(Box);
	Engine::GetGraphics().RemoveGeometry(Geo);
}

void AFruit::Tick()
{
	static const FPoint Velocity{ 0, 5 };

	if (!bStop)
	{
		Trans.Pos += Velocity;
	}
}

void AFruit::OnCollide(const AActor* Opponent)
{
	bStop = true;
}
