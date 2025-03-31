#pragma once

#include "Systems/Components.h"
#include "Game/Actor.h"
#include "Colliders.h"

#include <glm/glm.hpp>


typedef int32_t CBodyHandle;


struct FRigidBodyDesc
{
	EColliderShape ColliderShape;
	float Mass = 1.0f;
	float Restitution = 0.0f;
	float StaticFriction = 0.2f;
	float DynamicFriction = 0.2f;
	uint32_t Layers = 1;
};

struct FPhysicalMaterial
{
	float InvMass;
	float Restitution;
	float StaticFriction;
	float DynamicFriction;

	float GravityScale;
};

class CRigidBodyComp : public CComponent
{
	friend class SPhScene; // Make Physics Engine friend of Body

public:
	CRigidBodyComp(AActor* InOwner, CBodyHandle NewId, const FRigidBodyDesc& Desc);
	CRigidBodyComp(const CRigidBodyComp& Other);
	CRigidBodyComp(CRigidBodyComp&& Other) noexcept;
	
	~CRigidBodyComp();

	CRigidBodyComp& operator=(const CRigidBodyComp& Other);
	CRigidBodyComp& operator=(CRigidBodyComp&& Other) noexcept;

	template <class T, typename... Args>
	inline void SetupCollider(glm::vec2 InPivot, Args... args)
	{
		Shape = new T(InPivot, args...);
	}
	template <class T>
	inline const T& GetCollider()
	{
		return *static_cast<T*>(Shape);
	}

	inline void SetLayers(uint32_t InLayer)
	{
		Layers = InLayer;
	}

	// Current position fo each body!
	inline CTransform& GetTransform()
	{
		return GetOwner().GetTransform();
	}

	inline FBoxCollider GenerateAABB()
	{
		return Shape->GenerateAABB(GetTransform().GetPos());
	}

	inline void ApplyForce(glm::vec2 NewForce)
	{
		Forces += NewForce;
	}

private:
	CBodyHandle Id;

	// Collider
	FColliderShape* Shape = nullptr;

	FPhysicalMaterial PhyMat;
	uint32_t Layers;

	glm::vec2 Velocity;
	glm::vec2 Forces;

};

/* 
Materials
Rock       Density : 0.6  Restitution : 0.1
Wood       Density : 0.3  Restitution : 0.2
Metal      Density : 1.2  Restitution : 0.05
BouncyBall Density : 0.3  Restitution : 0.8
SuperBall  Density : 0.3  Restitution : 0.95
Pillow     Density : 0.1  Restitution : 0.2
Static     Density : 0.0  Restitution : 0.4
*/
