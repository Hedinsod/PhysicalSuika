#include "pch.h"
#include "Renderer.h"
#include "GeometryComp.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "Graphics/GfxBuffers.h"
#include "Graphics/GfxShader.h"


const std::string SRenderer::BasicVertexShader = R"(
			#version 330 core

			layout(location = 0) in vec2 a_Position;
			//layout(location = 1) in vec3 a_Color;
			//out vec3 v_Color;

			uniform mat4 u_ViewProj;
			uniform mat4 u_Model;

			void main()
			{
				//v_Color = a_Color;
				gl_Position = u_ViewProj * u_Model * vec4(a_Position, 0.0, 1.0);
			})";

const std::string SRenderer::BasicPixelShader = R"(
			#version 330 core

			layout(location = 0) out vec4 Color;
			//in vec3 v_Color;
				
			void main()
			{
				//Color = vec4(v_Color, 1.0);
				Color = vec4(0, 0, 0, 1.0);
			})";

std::shared_ptr<SGfxShader> SRenderer::Shader;

void SRenderer::Init()
{
	Shader = SGraphics::CreateShader(BasicVertexShader, BasicPixelShader);
}

void SRenderer::Begin(const std::shared_ptr<ACamera>& Camera)
{
	Shader->Bind();
	Shader->UploadUniform("u_ViewProj", Camera->GetVP());
}

void SRenderer::Sumbit(const CGeometry* Geo, const CTransform& Trans)
{
	Shader->UploadUniform("u_Model", Trans.GetModel());
	Geo->GetVertexData()->Bind();
	SGraphics::DrawIndexed(Geo->GetVertexData());
}
