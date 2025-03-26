#include "pch.h"
#include "GeometryComp.h"

#include "Graphics/Graphics.h"
#include "Graphics/GfxBuffers.h"


SGfxBufferLayout Mixed = {
	{ "Vertices", EGfxShaderData::Float3 },
	{ "Color", EGfxShaderData::Float3 },
};

SGfxBufferLayout Plain = {
	{ "Whatever", EGfxShaderData::Float3 }
};

CGeometry::CGeometry(AActor* InOwner)
	: CComponent(InOwner)
{
}

void CGeometry::SetVertices(const std::vector<float> InVertices)
{
	Vertices = InVertices;
}

void CGeometry::SetIndecies(const std::vector<uint32_t> InIndecies)
{
	Indecies = InIndecies;
}

void CGeometry::BuildGeometry()
{
	VertexData = SGraphics::CreateVertexData();
	VertexData->AddVertexData(Vertices, Plain);
	VertexData->SetIndexData({ 0, 1, 2 });
}
