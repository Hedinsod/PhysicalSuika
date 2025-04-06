#pragma once

#include "Systems/Components.h"
#include "Game/Actor.h"
#include "Colliders.h"
#include "Core/Event.h"

#include <glm/glm.hpp>

#include <list>


typedef int32_t CBodyHandle;

EVENT_OneParam(FRigidBodyEvent_OnDestruction, std::list<uint32_t> /*ContactsToDelete*/)
EVENT_OneParam(FRigidBodyEvent_OnCollision, AActor* /* Opponent */)

class CRigidBodyComp : public CComponent
{
	friend class SPhySolver; // Make Physics Engine friend of Body

public:
	// Constructors
	CRigidBodyComp(AActor* InOwner, uint32_t InMaterialId, FColliderShape* ShapeDesc, uint32_t Layers = 1);
	CRigidBodyComp(const CRigidBodyComp& Other);
	CRigidBodyComp(CRigidBodyComp&& Other) noexcept;
	
	~CRigidBodyComp();

	CRigidBodyComp& operator=(const CRigidBodyComp& Other);
	CRigidBodyComp& operator=(CRigidBodyComp&& Other) noexcept;

	inline void SetId(CBodyHandle NewId)
	{
		GAssert(Id == -1);
		Id = NewId;
	}

	// Getters
	template <class T>
	inline const T& GetCollider()
	{
		return *static_cast<T*>(Shape);
	}

	// 
	inline void SetLayers(uint32_t InLayer)
	{
		Layers = InLayer;
	}
	inline const uint32_t& GetLayers()
	{
		return Layers;
	}

	// 
	inline bool IsStatic()
	{
		return InvMass == 0;
	}

	// 
	inline void Disable()
	{
		bDisabled = true;
	}
	inline bool IsDisabled()
	{
		return bDisabled;
	}

	// 
	inline CBodyHandle GetId()
	{
		return Id;
	}

	// 
	inline FBoxCollider GenerateAABB()
	{
		return Shape->GenerateAABB(GetOwner().GetTransform().GetPos());
	}

	// 
	inline uint16_t GetShapeIndex()
	{
		return Shape->GetShapeIndex();
	}

	// Stuff
	void IntegrateVelocity(float TimeStep);
	void IntegratePosition(float TimeStep);

	// Contacts
	inline void AddContact(uint32_t ContactId)
	{
		Contacts.push_back(ContactId);
	}
	inline void RemoveContact(uint32_t ContactId)
	{
		Contacts.remove(ContactId);
	}

	// Events
	inline void SetOnDestructionEventHandler(const FRigidBodyEvent_OnDestruction::EventCallbackFn& InCallback)
	{
		OnDestruction.Subscribe(InCallback);
	}
	inline void SetOnCollisionEventHandler(const FRigidBodyEvent_OnCollision::EventCallbackFn& InCallback)
	{
		OnCollision.Subscribe(InCallback);
	}

private:
	void BasicCopy(const CRigidBodyComp& Other);

	// *** Data
	CBodyHandle Id = -1;

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

	bool bDisabled = false;

	std::list<uint32_t> Contacts;

	FRigidBodyEvent_OnDestruction OnDestruction;
	FRigidBodyEvent_OnCollision OnCollision;
};
