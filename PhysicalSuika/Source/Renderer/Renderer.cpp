#include "pch.h"
#include "Renderer.h"
#include "GeometryComp.h"
#include "Graphics/Graphics.h"
#include "Graphics/GfxBuffers.h"
#include "Graphics/GfxShader.h"


const std::string SRenderer::BasicVertexShader = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			//layout(location = 1) in vec3 a_Color;
			//out vec3 v_Color;

			//uniform mat4 u_ViewProj;

			void main()
			{
				//v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);

				//gl_Position = u_ViewProj * vec4(a_Position, 1.0);
			})";

const std::string SRenderer::BasicPixelShader = R"(
			#version 330 core

			layout(location = 0) out vec4 Color;
			in vec3 v_Color;
				
			void main()
			{
				Color = vec4(0.5, 0.5, 0.5, 1.0);
			})";

std::shared_ptr<SGfxShader> SRenderer::Shader;

void SRenderer::Init()
{
	Shader = SGraphics::CreateShader(BasicVertexShader, BasicPixelShader);
}

void SRenderer::Sumbit(CGeometry* Geo)
{
	Shader->Bind();
	Geo->GetVertexData()->Bind();
	SGraphics::DrawIndexed(Geo->GetVertexData());
}
