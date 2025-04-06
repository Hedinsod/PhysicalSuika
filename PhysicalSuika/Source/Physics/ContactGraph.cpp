#include "pch.h"
#include "ContactGraph.h"
#include "Systems/Engine.h"


int32_t FContactGraph::AddContact(CRigidBodyComp& InFirst, CRigidBodyComp& InSecond)
{
	uint64_t Hash = GenerateHash(InFirst.GetId(), InSecond.GetId());

	auto It = Tracker.find(Hash);
	if (It == Tracker.end())
	{
		Tracker.insert(Hash);

		int32_t ContactId = -1;
		if (InFirst.GetShapeIndex() == 1 && InSecond.GetShapeIndex() == 0)  // Circle-Box
		{
			ContactId = Contacts.Emplace(InSecond.GetId(), InFirst.GetId());
		}
		else
		{
			ContactId = Contacts.Emplace(InFirst.GetId(), InSecond.GetId());
		}

		Contacts[ContactId].Id = ContactId;
		Contacts[ContactId].Hash = Hash;

		// TODO: should be done once per body!
		InFirst.SetOnDestructionEventHandler(std::bind(&FContactGraph::RemoveAllContacts, this, std::placeholders::_1));
		InFirst.AddContact(ContactId);
		InSecond.SetOnDestructionEventHandler(std::bind(&FContactGraph::RemoveAllContacts, this, std::placeholders::_1));
		InSecond.AddContact(ContactId);

		return ContactId;
	}

	return -1;
}

void FContactGraph::RemoveContact(int32_t ContactId)
{
	FContact& Contact = Contacts[ContactId];

	CRigidBodyComp& FirstBody = Engine::GetPhyScene().GetRigidBody(Contact.FirstId);
	FirstBody.RemoveContact(Contact.Id);
	CRigidBodyComp& SecondBody = Engine::GetPhyScene().GetRigidBody(Contact.SecondId);
	SecondBody.RemoveContact(Contact.Id);

	Tracker.erase(Contact.Hash);
	Contacts.Remove(ContactId);
}

void FContactGraph::RemoveAllContacts(std::list<uint32_t> ContactsToDelete)
{
	for (uint32_t ContactId : ContactsToDelete)
	{
		RemoveContact(ContactId);
	}
}

uint64_t FContactGraph::GenerateHash(int32_t Id1, int32_t Id2)
{
	uint64_t Hash = 0;
	GAssert(Id1 != Id2);

	if (Id1 < Id2)
	{
		Hash |= (uint64_t)Id1 << 32;
		Hash |= Id2;
	}
	else
	{
		Hash |= (uint64_t)Id2 << 32;
		Hash |= Id1;
	}

	return Hash;
}
