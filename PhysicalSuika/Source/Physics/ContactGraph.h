#pragma once

#include "RigidBodyComp.h"
#include "Core/Containers/SparseArray.h"
#include <array>
#include <unordered_set>


// Achtung: This structure [probably] shuld be perfectly nullified
struct FManifold
{
	float        Penetration = 0;
	glm::vec2    Normal{ 0.0f };
	float        NormalImpulse = 0.0f;
	float        TangentImpulse = 0.0f;

	std::array<glm::vec2, 2> ContactPoints;
};

struct FContact
{
	FContact(CBodyHandle InFirstId, CBodyHandle InSecondId)
		: FirstId(InFirstId)
		, SecondId(InSecondId)
		, Hash(0)
		, Id(-1)
	{ }

	// Body 1 and Body 2
	CBodyHandle  FirstId = -1;
	CBodyHandle  SecondId = -1;

	// Description of the collision
	FManifold Manifold;

	// Unique hash/id to avoid duplications
	uint64_t Hash;
	int32_t Id;
};

// F is for [utility] structs, how to call utility classes ??
class FContactGraph
{
public:
	// Returns Contact Id if it was created
	// Returns -1 otherwise
	int32_t AddContact(CRigidBodyComp& InFirst, CRigidBodyComp& InSecond);
	
	// Deletes contact from Contacts AND all references from bodies
	void RemoveContact(int32_t ContactId);
	void RemoveAllContacts(std::list<uint32_t> ContactsToDelete);

	// Special remove all bodily contacts?

	// Iteration?
	TSparseArray<FContact>::Iterator begin() { return Contacts.begin(); }
	TSparseArray<FContact>::Iterator end() { return Contacts.end(); }
	
	int32_t Count() { return Contacts.Count(); }

private:
	uint64_t GenerateHash(int32_t Id1, int32_t Id2);
	// Stores contact hashes ensuring there are no duplicates
	std::unordered_set<uint64_t> Tracker;

	// THE contacts
	TSparseArray<FContact> Contacts;

};



