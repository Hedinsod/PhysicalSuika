#include "pch.h"
#include "Renderer.h"
#include "GeometryComp.h"
#include "Game/Camera.h"
#include "Graphics/Graphics.h"
#include "Graphics/GfxBuffers.h"
#include "Graphics/GfxShader.h"


StdShared<SGfxShader> SRenderer::Shader;

void SRenderer::Init()
{
	StdScoped<SGfxShaderFactory> NewFactory = SGraphics::GetShaderFactory();

	NewFactory->LoadSourceFromFile("Content/Shaders/BasicShader.glsl");
	Shader = NewFactory->Build();
}

void SRenderer::Begin(const StdShared<ACamera>& Camera)
{
	Shader->Bind();
	Shader->SetParameter("u_ViewProj", Camera->GetVP());
}

void SRenderer::Sumbit(const CGeometry* Geo, const CTransform& Trans, const FColorLinear& Color)
{
	Shader->SetParameter("u_Model", Trans.GetModel());
	Shader->SetParameter("u_Color", Color.ColorVec4);
	Geo->GetVertexData()->Bind();
	SGraphics::DrawIndexed(Geo->GetVertexData());
}
