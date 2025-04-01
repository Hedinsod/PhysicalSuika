#include "pch.h"
#include "Fruit.h"
#include "Systems/Engine.h"
#include "Physics/PhyMaterialLibrary.h"
#include "Renderer/GeometryComp.h"


bool AFruit::bMatCreated = false;
uint32_t AFruit::MatId = 0;

AFruit::AFruit(glm::vec2 InPos)
	: AActor(InPos, "Fruit\n")
{
	FColorRGB Color({ 250, 5, 5 });
	Geo = Engine::GetGraphics().CreateGeometry(this);
	
	std::vector<float> Points;
	std::vector<uint32_t> Inds;
	uint32_t i = 0;
	Points.push_back(0.0f);
	Points.push_back(0.0f);
	Inds.push_back(i++);
	for (float a = 0.f; a <= 360.f; a += 5.f)
	{
		Points.push_back(glm::sin(glm::radians(a)));
		Points.push_back(glm::cos(glm::radians(a)));
		Inds.push_back(i++);
	}
	Inds.push_back(1);
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

	if (!bMatCreated)
	{
		FPhysicalMaterial Berry{ .Density = 0.0955f, .Friction = 0.2f, .GravityScale = 2.0f };
		MatId = SPhyMatirialLibrary::AddMaterial(Berry);
		bMatCreated = true;
	}

	FColliderShape* Shape = FColliderShape::Create<FCircleCollider>({ 0, 0 }, 1.f);
	Box = Engine::GetPhyScene().CreateRigidBody(this, MatId, Shape);
}

AFruit::~AFruit()
{
	Engine::GetPhyScene().RemoveRigidBody(Box);
	Engine::GetGraphics().RemoveGeometry(Geo);
}

void AFruit::Tick()
{
}
