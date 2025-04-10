#include "pch.h"
#include "GeometryComp.h"

#include "Graphics/Graphics.h"
#include "Graphics/GfxBuffers.h"


SGfxBufferLayout Mixed = {
	{ "Vertices", EGfxShaderData::Float3 },
	{ "Color", EGfxShaderData::Float3 },
};

SGfxBufferLayout Plain2 = {
	{ "Vertices", EGfxShaderData::Float2 }
};

/*
SGfxBufferLayout Plain3 = {
	{ "Whatever", EGfxShaderData::Float3 }
};
*/

CGeometry::CGeometry(AActor* InOwner)
	: CComponent(InOwner)
	, Color(1.0f, 0.0f, 1.0f) // Eye bleeding magenta
{
}

void CGeometry::SetVertices(const std::vector<float>& InVertices)
{
	Vertices = InVertices;
}

void CGeometry::SetIndices(const std::vector<uint32_t>& InIndices)
{
	Indices = InIndices;
}

void CGeometry::BuildGeometry()
{
	VertexData = SGraphics::CreateVertexData();
	VertexData->AddVertexData(Vertices, Plain2);
	VertexData->SetIndexData(Indices);
}
