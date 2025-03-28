#include "pch.h"
#include "Fruit.h"
#include "Systems/Engine.h"
#include "Renderer/GeometryComp.h"


AFruit::AFruit(glm::vec2 InPos)
	: AActor(InPos, "Fruit\n")
{
	FColorRGB Color({ 250, 5, 5 });
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

	GeoBox = Engine::GetGraphics().CreateGeometry(this);
	GeoBox->SetVertices({
		-10,  10,
		 10,  10,
		 10, -10,
		-10, -10});
	GeoBox->SetIndecies({
		0, 1, 2, 3, 0
		});
	GeoBox->BuildGeometry();

	Box = Engine::GetCollision().AddPhysics(this, 0.4f, 0.4f);
	Box->SetupBoxCollider({ 0, 0 }, -10, 10, 10, -10);
	Box->SetLayer(1);

	//Box->Velocity = { 0, -1.0f }; // manually set initial velocity
}

AFruit::~AFruit()
{
	Engine::GetCollision().RemovePhysics(Box);
	Engine::GetGraphics().RemoveGeometry(Geo);
	Engine::GetGraphics().RemoveGeometry(GeoBox);
}

void AFruit::Tick()
{
	/*
	static const glm::vec2 Velocity{ 0, -1.0f };

	if (!bStop)
	{
		Trans.Translate(Velocity);
	}
	*/
}

void AFruit::OnCollide(AActor& Opponent)
{
	bStop = true;
}
