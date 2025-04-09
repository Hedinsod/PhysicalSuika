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
	friend class SPhySolver;  // Solver will read all parameters directly

public:
	// Constructors
	CRigidBodyComp(AActor* InOwner, const std::string& MaterialTag, FColliderShape* ShapeDesc, uint32_t Layers = 1);
	CRigidBodyComp(const CRigidBodyComp& Other);
	CRigidBodyComp(CRigidBodyComp&& Other) noexcept;
	
	~CRigidBodyComp();

	CRigidBodyComp& operator=(const CRigidBodyComp& Other);
	CRigidBodyComp& operator=(CRigidBodyComp&& Other) noexcept;

	// For initialization only
	inline void SetId(CBodyHandle NewId)
	{
		GAssert(Id == -1);
		Id = NewId;
	}
	inline CBodyHandle GetId() { return Id; }

	// Accessors
	inline void SetLayers(uint32_t InLayer)	{ Layers = InLayer; }
	inline const uint32_t& GetLayers() { return Layers; }

	inline bool IsStatic() { return InvMass == 0; }
	
	inline void Enable() { bDisabled = false; }
	inline void Disable() { bDisabled = true; }
	inline bool IsDisabled() { return bDisabled; }
	
	// Colliders
	template <class T>
	inline const T& GetCollider() { return *static_cast<T*>(Shape); }
	inline uint16_t GetShapeIndex() { return Shape->GetShapeIndex(); }
	inline FBoxCollider GenerateAABB() { return Shape->GenerateAABB(GetOwner().GetTransform().GetPos()); }

	// 
	void IntegrateVelocity(float TimeStep);
	void IntegratePosition(float TimeStep);

	// Contacts
	inline void AddContact(uint32_t ContactId) { Contacts.push_back(ContactId); }
	inline void RemoveContact(uint32_t ContactId) { Contacts.remove(ContactId); }

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

	// Index in the riged body array
	CBodyHandle Id = -1;

	// Collider
	FColliderShape* Shape = nullptr;
	std::string MaterialTag;
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

	// Status - should the body be solved
	bool bDisabled = false;

	// List of all cointacts with other bodies
	std::list<uint32_t> Contacts;

	// Events
	FRigidBodyEvent_OnDestruction OnDestruction;
	FRigidBodyEvent_OnCollision OnCollision;

};
