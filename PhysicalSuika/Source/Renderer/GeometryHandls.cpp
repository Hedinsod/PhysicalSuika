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
	: Id(-1)
{
	Dummy = MakeScoped<AActor>(glm::vec2(0.0,0.0));
}

FPrimitiveObject::FPrimitiveObject(FPrimitiveObject&& Other) noexcept
	: Id(Other.Id)
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
	Id = Other.Id;
	Other.Id = -1;

	return *this;
}
FPrimitiveObject::~FPrimitiveObject()
{
	Erase();
}
void FPrimitiveObject::Erase()
{
	if (IsValid())
	{
		Engine::Renderer().Overlay.Remove(Id);
		Dummy.reset();
		Id = -1;
	}
}

void FPrimitiveObject::SetPos(const glm::vec2& InPos)
{
	Dummy->GetTransform().SetPos(InPos);
}
