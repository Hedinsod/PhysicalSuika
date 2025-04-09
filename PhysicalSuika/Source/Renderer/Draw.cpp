#include "pch.h"

#include "Draw.h"
#include "Game/Actor.h"
#include "Renderer/Renderer.h"
#include "Renderer/GeometryComp.h"
#include "Graphics/Graphics.h"
#include "Graphics/GfxContext.h"


CGeometry* SDraw::CreateGeometry(AActor* InOwner)
{
	CGeometry* Elem = new CGeometry(InOwner);
	GeometryPool.insert(Elem);

	return Elem;
}
void SDraw::RemoveGeometry(CGeometry* Geo)
{
	GeometryPool.erase(Geo);
	delete Geo;
}

void SDraw::Tick(const std::shared_ptr<ACamera>& Camera)
{
	SRenderer::Begin(Camera);
	for (CGeometry* Geo : GeometryPool)
	{
		SRenderer::Sumbit(Geo, Geo->GetOwner().GetTransform());
	}
}


