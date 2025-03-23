#include "pch.h"

#include "Draw.h"
#include "Graphics/GfxContext.h"
#include "Game/Entity.h"
#include "Graphics/Graphics.h"


CGeometry* SDraw::CreateGeometry(Entity* InOwner)
{
	CGeometry* Elem = new CGeometry(InOwner);
	GeometryPool.insert(Elem);

	return Elem;
}
void SDraw::RemoveGeometry(CGeometry* Geo)
{
	GeometryPool.erase(Geo);
}

void SDraw::Tick()
{
	auto RotatePoint = [](FPoint& P, FPoint Rotation)
		{
			float x = P.x;
			float y = P.y;

			P.x = x * Rotation.x - y * Rotation.y;
			P.y = x * Rotation.y + y * Rotation.x;
		};

	Context->FillRect({ 0, 0 }, { 800, 600 }, FColor{ 0, 0, 0 });

	for (CGeometry* Geo : GeometryPool)
	{
		for (int i = 0; i < Geo->VertexData.size()-1; i++)
		{
			FColor Color = Geo->VertexData[i].Color;

			FPoint Start = Geo->VertexData[i].Vertex;
			RotatePoint(Start, Geo->Owner->GetTransform().Rot);
			Start += Geo->Owner->GetTransform().Pos;

			FPoint End = Geo->VertexData[i + 1].Vertex;
			RotatePoint(End, Geo->Owner->GetTransform().Rot);
			End += Geo->Owner->GetTransform().Pos;

			Context->DrawLine(Start, End, Color);
		}
	}
}


