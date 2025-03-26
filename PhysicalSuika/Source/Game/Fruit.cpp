#include "pch.h"
#include "Fruit.h"
#include "Systems/Engine.h"
#include "Renderer/GeometryComp.h"

AFruit::AFruit(float InX, float InY)
	: AActor(InX, InY)
{
	FColor Color({ 250, 5, 5 });
	Geo = Engine::GetGraphics().CreateGeometry(this);
	Geo->SetVertices({ 
		  -5,  10,
		 -10,   0,
		  -5, -10,
		   5, -10,
		  10,   0,
		   5, +10,
		  -5, +10
		});
	Geo->SetIndecies({
		0, 1, 2, 3, 4, 5, 6
		});
	Geo->BuildGeometry();

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
