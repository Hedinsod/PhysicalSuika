#pragma once

#include "Systems/Components.h"
#include "Game/Actor.h"
#include "Colliders.h"

#include <glm/glm.hpp>


typedef int32_t CBodyHandle;

class CRigidBodyComp : public CComponent
{
	friend class SPhySolver; // Make Physics Engine friend of Body

public:
	// Constructors
	CRigidBodyComp(AActor* InOwner, CBodyHandle NewId, uint32_t InMaterialId, FColliderShape* ShapeDesc, uint32_t Layers = 1);
	CRigidBodyComp(const CRigidBodyComp& Other);
	CRigidBodyComp(CRigidBodyComp&& Other) noexcept;
	
	~CRigidBodyComp();

	CRigidBodyComp& operator=(const CRigidBodyComp& Other);
	CRigidBodyComp& operator=(CRigidBodyComp&& Other) noexcept;

	// Getters
	template <class T>
	inline const T& GetCollider()
	{
		return *static_cast<T*>(Shape);
	}
	inline void SetLayers(uint32_t InLayer)
	{
		Layers = InLayer;
	}
	inline const uint32_t& GetLayers()
	{
		return Layers;
	}

	inline bool IsStatic()
	{
		return InvMass == 0;
	}
	inline CBodyHandle GetId()
	{
		return Id;
	}

	// TODO: Current position for each body!
	inline CTransform& GetTransform()
	{
		return GetOwner().GetTransform();
	}

	inline FBoxCollider GenerateAABB()
	{
		return Shape->GenerateAABB(GetTransform().GetPos());
	}

	// Stuff
	void IntegrateVelocity(float TimeStep);
	void IntegratePosition(float TimeStep);

private:
	void BasicCopy(const CRigidBodyComp& Other);

	// *** Data
	CBodyHandle Id;

	// Collider
	FColliderShape* Shape = nullptr;
	uint32_t MaterialId;
	uint32_t Layers = 1;

	// Calculatable
	float InvMass = 0.0f;
	float InvInertia = 0.0f;

	// Dynamic parameters
	glm::vec2 Position{ 0 };
	glm::vec2 Velocity{ 0 };
	glm::vec2 Forces{ 0 };

	float Orientation = 0.0f;
	float AngularVelocity = 0.0f;
	float Torque = 0.0f;
};
