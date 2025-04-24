#include "pch.h"
#include "GeometryHandle.h"

#include "Systems/Engine.h"

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
