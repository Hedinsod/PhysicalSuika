#pragma once

#include "RigidBodyComp.h"





struct FEdge
{
	CBodyHandle BodyId;

	uint32_t Prev;    // index in Edge Array / Contact Array
	uint32_t Next;    // index in Edge Array / Contact Array
};

struct FContact
{
	// some data

	FEdge Edges[2];


};



