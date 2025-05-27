#include "pch.h"
#include "GeometryHandls.h"
#include "Systems/Engine.h"


// ****************************************************************************
// ********** FGeometryHandle *************************************************
// ****************************************************************************

FGeometryHandle FGeometryHandle::Create(AActor* InOwner)
{
	int32_t Id = Engine::Renderer().GeometryPool.Emplace(InOwner);

	return FGeometryHandle(Id);
}
void FGeometryHandle::Erase()
{
	GAssert(Id >= 0);

	Engine::Renderer().GeometryPool.Remove(Id);
	Id = -1;
}

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
// ********** FPrimitiveObject ************************************************
// ****************************************************************************

FPrimitiveObject::FPrimitiveObject()
	: OwningPool(nullptr)
	, Id(-1)
{
	Dummy = MakeScoped<AActor>(glm::vec2(0.0, 0.0));
}

FPrimitiveObject::FPrimitiveObject(TSparseArray<CGeometry>& InOwningPool)
	: OwningPool(&InOwningPool)
	, Id(-1)
{
	Dummy = MakeScoped<AActor>(glm::vec2(0.0,0.0));
	Id = OwningPool->Emplace(Dummy.get());
}

FPrimitiveObject::FPrimitiveObject(FPrimitiveObject&& Other) noexcept
	: OwningPool(Other.OwningPool)
	, Id(Other.Id)
	, Dummy(std::move(Other.Dummy))
{
	Other.Id = -1;
}
FPrimitiveObject& FPrimitiveObject::operator=(FPrimitiveObject&& Other) noexcept
{
	if (this == &Other)
		return *this;

	Erase();

	Dummy = std::move(Other.Dummy);
	OwningPool = Other.OwningPool;
	Id = Other.Id;
	Other.Id = -1;

	return *this;
}
FPrimitiveObject::~FPrimitiveObject()
{
	Erase();
}
void FPrimitiveObject::Create(TSparseArray<CGeometry>& InOwningPool)
{
	OwningPool = &InOwningPool;
	Id = OwningPool->Emplace(Dummy.get());
}
void FPrimitiveObject::Erase()
{
	if (IsValid())
	{
		OwningPool->Remove(Id);
		Dummy.reset();
		Id = -1;
	}
}

CGeometry& FPrimitiveObject::operator*()
{
	return (*OwningPool)[Id];
}

const CGeometry& FPrimitiveObject::operator*() const
{
	return (*OwningPool)[Id];
}

CGeometry* FPrimitiveObject::operator->()
{
	return &(*OwningPool)[Id];
}

const CGeometry* FPrimitiveObject::operator->() const
{
	return &(*OwningPool)[Id];
}

void FPrimitiveObject::SetPos(const glm::vec2& InPos)
{
	Dummy->GetTransform().SetPos(InPos);
}
