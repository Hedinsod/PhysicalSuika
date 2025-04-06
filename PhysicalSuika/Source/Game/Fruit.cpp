#include "pch.h"
#include "Fruit.h"
#include "Game.h"
#include "Systems/Engine.h"
#include "Physics/PhyMaterialLibrary.h"
#include "Renderer/GeometryComp.h"

#include <sstream>


bool AFruit::bMatCreated = false;
uint32_t AFruit::MatId = 0;

AFruit::AFruit(glm::vec2 InPos, EFruitType InType)
	: AActor(InPos, "Fruit\n")
	, Type(InType)
{
	float Scale = 0.4f + 0.15f * (int16_t)Type;
	Trans.SetScale({ Scale, Scale });

	// Graphics
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
	Geo->BuildGeometry();

	// Physical Material
	if (!bMatCreated)
	{
		FPhysicalMaterial Berry{ .Density = 0.0955f, .Friction = 0.2f, .GravityScale = 2.0f };
		MatId = SPhyMatirialLibrary::AddMaterial(Berry);
		bMatCreated = true;
	}

	FColliderShape* Shape = FColliderShape::Create<FCircleCollider>({ 0, 0 }, Scale);
	Box = Engine::GetPhyScene().CreateRigidBody(this, MatId, Shape);

	Engine::GetPhyScene().GetRigidBody(Box).SetOnCollisionEventHandler(std::bind(&AFruit::Disappear, this, std::placeholders::_1));
}

AFruit::~AFruit()
{
	Engine::GetPhyScene().RemoveRigidBody(Box);
	Engine::GetGraphics().RemoveGeometry(Geo);
}

void AFruit::Tick()
{
}

void AFruit::Disappear(AActor* Opponent)
{
	AFruit* Other = dynamic_cast<AFruit*>(Opponent);

	if (Other && Other->Type == Type && Type != EFruitType::Watermelon)
	{
		bPendingDelete = true;

		/*
		std::string str;
		std::stringstream s(str);
		s << "Contact " << (int16_t)Other->Type << " & " << (int16_t)Type;
		Utility::Log(s.str());
		*/

		// TODO: Disable physics // Other will be disabled by itself
		Engine::GetPhyScene().GetRigidBody(Box).Disable();

		// TODO: Spawn next type
		if (Engine::GetPhyScene().GetRigidBody(Other->Box).IsDisabled())
		{
			glm::vec2 Spawn = (Trans.GetPos() + Other->Trans.GetPos()) * 0.5f;
			GGame->AddEntity<AFruit>(Spawn, (EFruitType)(int16_t(Type) + 1));
		}
	}
}
