#include "pch.h"

#include "Renderer.h"
#include "Game/Actor.h"
#include "Game/Camera.h"

#include "Systems/Engine.h"
#include "Graphics/Graphics.h"
#include "Core/Input.h"


// ****************************************************************************
// ********** SRenderer *******************************************************
// ****************************************************************************

// Just random assumption
static const uint32_t MaxTriangles = 100;
static const uint32_t MaxVertices = MaxTriangles * 3;
static const uint32_t MaxIndices = MaxTriangles * 3;
static const uint32_t MaxTextures = 8;

SRenderer::SRenderer()
{
	// Make shader
	StdScoped<SGfxShaderFactory> ShaderFactory = SGraphics::GetShaderFactory();

	ShaderFactory->LoadSourceFromFile("Content/Shaders/BatchShader.glsl");
	Shader = ShaderFactory->Build();

	// Texture slots
	std::array<int32_t, MaxTextures> Samplers;
	for (int i = 0; i < Samplers.size(); i++) { Samplers[i] = i; }
	Shader->Bind();
	Shader->SetParameter("u_Textures", Samplers.data(), static_cast<uint32_t>(Samplers.size()));

	// Prepare VBO, IBO, VAO:
	StdScoped<SGfxBufferFactory> BuffersFactory = SGraphics::GetBufferFactory();

	VAO = BuffersFactory->CreateVertexArray();
	VBO = BuffersFactory->CreateVertexBuffer(MaxVertices);
	IBO = BuffersFactory->CreateIndexBuffer(MaxIndices);

	VAO->Attach(VBO);
	VAO->Attach(IBO);
	VAO->SetLayout({ { "Position", EGfxShaderData::Float4 },
					 { "Color", EGfxShaderData::Float4 },
					 { "TexCoord", EGfxShaderData::Float2 },
					 { "Texture", EGfxShaderData::Float } });

	VertexData = new FVertex[MaxVertices];
	IndexData = new uint32_t[MaxIndices];
	NextVertex = 0;
	NextIndex = 0;
	IndexOffset = 0;

}

SRenderer::~SRenderer()
{
	delete[] VertexData;
	delete[] IndexData;
}

void SRenderer::Begin(const StdShared<ACamera>& Camera)
{
	Shader->SetParameter("u_ViewProj", Camera->GetVP());

	NextVertex = 0;
	NextIndex = 0;
	IndexOffset = 0;

	TextureToSlot.clear();
	NextTexSlot = 0;
}

void SRenderer::RenderPool(TSparseArray<CGeometry>& Pool)
{
	for (const CGeometry& Geo : Pool)
	{
		if (NextIndex + Geo.Indices.size() >= MaxIndices)
		{
			Finish();

			// Reset
			NextVertex = 0;
			NextIndex = 0;
			IndexOffset = 0;

			TextureToSlot.clear();
			NextTexSlot = 0;
		}

		const glm::mat4& Model = Geo.GetOwner().GetTransform().GetModel();
		const FMaterial& Material = Engine::GetMaterialLibrary().Get(Geo.MaterialTag);

		float TexSlot = 0.0f;
		auto It = TextureToSlot.find(Material.Texture);
		if (It == TextureToSlot.end())
		{
			TexSlot = static_cast<float>(NextTexSlot);

			Material.Texture->Bind(NextTexSlot);
			TextureToSlot.emplace(Material.Texture, TexSlot);
			
			NextTexSlot++;
		}
		else
		{
			TexSlot = It->second;
		}

		for (uint32_t i = 0; i < Geo.Vertices.size(); i++)
		{
			const glm::vec4& Vertex = Geo.Vertices[i];
			const glm::vec2& UV = Geo.UVs[i];

			GAssert(NextVertex < MaxVertices);

			VertexData[NextVertex].Position = Model * Vertex;
			VertexData[NextVertex].Color = Material.Color;
			VertexData[NextVertex].TexCoord = UV;
			VertexData[NextVertex].TexSlot = TexSlot;
			NextVertex++;
		}

		for (const uint32_t& Index : Geo.Indices)
		{
			GAssert(NextIndex < MaxIndices);

			IndexData[NextIndex++] = IndexOffset + Index;
		}

		IndexOffset += static_cast<uint32_t>(Geo.Vertices.size());
	}
}

void SRenderer::Finish()
{
	VBO->UploadVertices(VertexData, NextVertex * sizeof(FVertex));
	IBO->UploadIndices(IndexData, NextIndex * sizeof(uint32_t));

	VAO->Bind();

	SGraphics::DrawIndexed(NextIndex);
}
