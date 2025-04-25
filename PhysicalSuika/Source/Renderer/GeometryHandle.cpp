#include "pch.h"
#include "GeometryHandle.h"

#include "Systems/Engine.h"

// ****************************************************************************
// ********** FGeometryHandle ************************************************
// ****************************************************************************

CGeometry& FGeometryHandle::operator*()
{
	return Engine::Renderer().GeometryPool[Id];
}

const CGeometry& FGeometryHandle::operator*() const
{
	return Engine::Renderer().GeometryPool[Id];
}

CGeometry* FGeometryHandle::operator->()
{
	return &Engine::Renderer().GeometryPool[Id];
}

const CGeometry* FGeometryHandle::operator->() const
{
	return &Engine::Renderer().GeometryPool[Id];
}


// ****************************************************************************
// ********** FPrimitiveHandle ************************************************
// ****************************************************************************

FPrimitiveHandle::~FPrimitiveHandle()
{
	if (Id != -1)
	{
		Engine::Renderer().RemovePrimitive(*this);
	}
}

FPrimitiveHandle::FPrimitiveHandle(FPrimitiveHandle&& Other) noexcept
	: Id(Other.Id)
{
	Other.Id = -1;
}

FPrimitiveHandle& FPrimitiveHandle::operator=(FPrimitiveHandle&& Other) noexcept
{
	if (this == &Other)
		return *this;

	if (Id != -1)
	{
		Engine::Renderer().RemovePrimitive(*this);
	}
	Id = Other.Id;

	Other.Id = -1;

	return *this;
}
