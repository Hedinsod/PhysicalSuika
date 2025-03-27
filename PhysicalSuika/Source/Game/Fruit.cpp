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

	Geo->SetColors({
		0.8f, 0.1f, 0.1f,
		0.8f, 0.1f, 0.1f,
		0.8f, 0.1f, 0.1f,
		0.8f, 0.1f, 0.1f,
		0.8f, 0.1f, 0.1f,
		0.8f, 0.1f, 0.1f,
		0.8f, 0.1f, 0.1f
		});

	Geo->BuildGeometry();

	Box = Engine::GetCollision().AddPhysics(this);
	Box->SetupBoundingBox(-10, 10, 10, -10);
}

AFruit::~AFruit()
{
	Engine::GetCollision().RemovePhysics(Box);
	Engine::GetGraphics().RemoveGeometry(Geo);
}

void AFruit::Tick()
{
	static const glm::vec2 Velocity{ 0, -1.0f };

	if (!bStop)
	{
		Trans.Translate(Velocity);
	}
}

void AFruit::OnCollide(AActor& Opponent)
{
	bStop = true;
}
