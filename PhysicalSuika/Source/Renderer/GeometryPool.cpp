#include "pch.h"

#include "GeometryPool.h"
#include "Game/Actor.h"
#include "Game/Camera.h"

#include "Systems/Engine.h"
#include "Graphics/Graphics.h"


static const int32_t MaxHexagones = 1000;
static const int32_t MaxVertices = MaxHexagones * 6;
static const int32_t MaxIndices = MaxHexagones * 7;

SGeometryPool::SGeometryPool()
{
	// Make shader
	StdScoped<SGfxShaderFactory> ShaderFactory = SGraphics::GetShaderFactory();

	ShaderFactory->LoadSourceFromFile("Content/Shaders/BatchShader.glsl");
	Shader = ShaderFactory->Build();

	// Prepare VBO, IBO, VAO:
	StdScoped<SGfxBufferFactory> BuffersFactory = SGraphics::GetBufferFactory();
	VBO = BuffersFactory->CreateVertexBuffer(MaxVertices);
	IBO = BuffersFactory->CreateIndexBuffer(MaxIndices);
	//RenderVAO = BuffersFactory->CreateVertexArray(VBO, IBO, {{ "Vertices", EGfxShaderData::Float3 }});

	VertexData = new FVertex[MaxVertices];
	IndexData = new uint32_t[MaxIndices];
	NextVertex = 0;
	NextIndex = 0;
	IndexOffset = 0;
}

SGeometryPool::~SGeometryPool()
{
	delete[] VertexData;
	delete[] IndexData;
}

FGeometryHandle SGeometryPool::CreateGeometry(AActor* InOwner)
{
	int32_t Id = GeometryPool.Emplace(InOwner);

	return FGeometryHandle(Id);
}
void SGeometryPool::RemoveGeometry(FGeometryHandle GeoId)
{
	GeometryPool.Remove(GeoId.Id);
}

void SGeometryPool::Begin(const StdShared<ACamera>& Camera)
{
	Shader->Bind();
	Shader->SetParameter("u_ViewProj", Camera->GetVP());

	NextVertex = 0;
	NextIndex = 0;
	IndexOffset = 0;
}

void SGeometryPool::Tick()
{
	if (NextIndex >= MaxIndices)
	{
		return;
		// Restart
	}

	for (CGeometry& Geo : GeometryPool)
	{
		const glm::mat4& Model = Geo.GetOwner().GetTransform().GetModel();
		const FMaterial& Material = Engine::GetMaterialLibrary().Get(Geo.MaterialTag);
		for (const glm::vec4& Vertex : Geo.Vertices)
		{
			VertexData[NextVertex].Position = Model * Vertex;
			GAssert(VertexData[NextVertex].Position.z == 0.0f);

			VertexData[NextVertex].Color = Material.Color;
			NextVertex++;
		}

		for (const uint32_t& Index : Geo.Indices)
		{
			IndexData[NextIndex++] = IndexOffset + Index;
		}

		IndexOffset += static_cast<uint32_t>(Geo.Vertices.size());
	}
}

void SGeometryPool::Finish()
{
	VBO->UploadVertices(VertexData, NextVertex * sizeof(FVertex));
	VBO->SetLayout({ { "Position", EGfxShaderData::Float4 },
	                 { "Color", EGfxShaderData::Float4 } });
	IBO->UploadIndices(IndexData, NextIndex * sizeof(uint32_t));

	SGraphics::DrawIndexed(NextIndex);
}
