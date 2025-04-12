#include "pch.h"
#include "GeometryHandle.h"

#include "Systems/Engine.h"

CGeometry& FGeometryHandle::operator*()
{
	return Engine::GetGraphics().GeometryPool[Id];
}

const CGeometry& FGeometryHandle::operator*() const
{
	return Engine::GetGraphics().GeometryPool[Id];
}

CGeometry& FGeometryHandle::operator->()
{
	return Engine::GetGraphics().GeometryPool[Id];
}

const CGeometry& FGeometryHandle::operator->() const
{
	return Engine::GetGraphics().GeometryPool[Id];
}
