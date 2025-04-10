#include "pch.h"

#include "GeometryPool.h"
#include "Game/Actor.h"
#include "Renderer/Renderer.h"
#include "Renderer/GeometryComp.h"
#include "Graphics/Graphics.h"
#include "Graphics/GfxContext.h"


CGeometry* SGeometryPool::CreateGeometry(AActor* InOwner)
{
	CGeometry* Elem = new CGeometry(InOwner);
	GeometryPool.insert(Elem);

	return Elem;
}
void SGeometryPool::RemoveGeometry(CGeometry* Geo)
{
	GeometryPool.erase(Geo);
	delete Geo;
}

void SGeometryPool::Tick(const StdShared<ACamera>& Camera)
{
	SRenderer::Begin(Camera);
	for (CGeometry* Geo : GeometryPool)
	{
		SRenderer::Sumbit(Geo, Geo->GetOwner().GetTransform(), Geo->GetColor());
	}
}


