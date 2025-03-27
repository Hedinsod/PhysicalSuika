#include "pch.h"
#include "Fruit.h"
#include "Systems/Engine.h"
#include "Renderer/GeometryComp.h"


AFruit::AFruit(glm::vec2 InPos)
	: AActor(InPos, "Fruit\n")
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
	
	/*
	Geo->SetVertices({
		 -0.8,    0,
		    0,  0.8,
		  0.8,    0,
		    0, -0.8
		});
	Geo->SetIndecies({
		0, 1, 2, 3
		});
		*/
	
	/*
	Geo->SetIndecies({
		0, 1, 2, 2, 3, 0
		});
		*/

	Geo->BuildGeometry();

	Box = new CollisionBox(this, 10, -10, 10, -10);
	Engine::GetCollision().AddCollisionBox(Box);
}

AFruit::~AFruit()
{
	Engine::GetCollision().RemoveCollisionBox(Box);
	delete Box;
	Engine::GetGraphics().RemoveGeometry(Geo);
}

void AFruit::Tick()
{
	static const glm::vec2 Velocity{ 0, -1 };

	if (!bStop)
	{
		Trans.Translate(Velocity);
	}
}

void AFruit::OnCollide(const AActor* Opponent)
{
	bStop = true;
}
