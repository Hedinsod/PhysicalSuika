#include "pch.h"
#include "Fruit.h"
#include "Systems/Engine.h"
#include "Renderer/GeometryComp.h"


AFruit::AFruit(glm::vec2 InPos)
	: AActor(InPos, "Fruit\n")
{
	FColorRGB Color({ 250, 5, 5 });
	Geo = Engine::GetGraphics().CreateGeometry(this);
	
	std::vector<float> Points;
	std::vector<uint32_t> Inds;
	uint32_t i = 0;
	for (float a = 0.f; a <= 360.f; a += 5.f)
	{
		Points.push_back(glm::sin(glm::radians(a)));
		Points.push_back(glm::cos(glm::radians(a)));
		Inds.push_back(i++);
	}
	Inds.push_back(0);
	Geo->SetVertices(Points);
	Geo->SetIndecies(Inds);
	/*
	Geo->SetVertices({
		 -0.5f,  1.f,
		 -1.0f,  0.f,
		 -0.5f, -1.f,
		  0.5f, -1.f,
		  1.0f,  0.f,
		  0.5f,  1.f,
		 -0.5f,  1.f
		});
		*/
	/*Geo->SetIndecies({
		0, 1, 2, 3, 4, 5, 6
		});*/
	Geo->BuildGeometry();

	FRigidBodyDesc Desc;
	Desc.ColliderShape = EColliderShape::Box;
	Desc.Mass = 0.3f;
	Desc.Restitution = 0.1f;
	Desc.StaticFriction = 0.3f;
	Desc.DynamicFriction = 0.1f;
	Desc.Layers = 1;
	Box = Engine::GetCollision().CreateRigidBody(this, Desc);

	// TODO: fix this!
	CRigidBodyComp& Body = Engine::GetCollision().GetRigidBody(Box);
	Body.SetupCollider<FCircleCollider>({ 0, 0 }, 1.f);
}

AFruit::~AFruit()
{
	Engine::GetCollision().RemoveRigidBody(Box);
	Engine::GetGraphics().RemoveGeometry(Geo);
}

void AFruit::Tick()
{
}
