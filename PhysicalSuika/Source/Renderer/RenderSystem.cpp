#include "pch.h"

#include "RenderSystem.h"
#include "GeometryHandls.h"
#include "Systems/Engine.h"
#include "Core/Input.h"


SRenderSystem::SRenderSystem()
{
	Renderer = MakeScoped<SRenderer>();

	Font = MakeScoped<SFont>();

	StdShared<SGfxTexture> FontAtlas = Font->Prepare(GApp->GetSettings().FontPath);
	Engine::GetMaterialLibrary().Add("FontAtlas", MakeShared<FMaterial>(/*.Density*/ 0.0f, /*.Friction*/ 0.0f, /*.GravityScale*/ 0.0f, /*Color*/glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), FontAtlas));
}

void SRenderSystem::Tick()
{
	Renderer->Begin(CurrentCamera);
	Renderer->RenderPool(GeometryPool);
	Renderer->Finish();

	Renderer->Begin(CurrentCamera);
	Renderer->RenderPool(Overlay);
	Renderer->Finish();

	if (SInput::IsButtonPressed(EInputCode::Tab))
	{
		Renderer->Begin(CurrentCamera);
		Renderer->RenderPool(DebugOverlay);
		Renderer->Finish();
	}
}

FPrimitiveObject SRenderSystem::DrawDot(const glm::vec2& Point, float Size)
{
	FPrimitiveObject NewPrimitive(DebugOverlay);
	NewPrimitive.SetPos(Point);

	glm::vec2 xShift(Size, 0.0f);
	glm::vec2 yShift(0.0f, Size);

	NewPrimitive->SetVertices({ glm::vec2(0.0f, 0.0f), xShift, yShift});
	NewPrimitive->SetIndices({ 0, 1, 2 });
	NewPrimitive->SetMaterial("Overlay");

	return NewPrimitive;
}

FPrimitiveObject SRenderSystem::DrawLine(const glm::vec2& Start, const glm::vec2& Finish, float Size)
{
	static AActor Dummy(glm::vec2(0.0f, 0.0f));
	Size /= 2;

	FPrimitiveObject NewPrimitive(Overlay);

	glm::vec2 yShift(0.0f, Size);

	NewPrimitive->SetVertices({ Start - yShift, Finish - yShift, Finish + yShift, Start - yShift });
	NewPrimitive->SetIndices({ 0, 1, 2, 2, 3, 0 });
	NewPrimitive->SetMaterial("Overlay");

	return NewPrimitive;
}

FPrimitiveObject SRenderSystem::DrawBox(const glm::vec2& LeftUpper, const glm::vec2& RightLower)
{
	FPrimitiveObject NewPrimitive(Overlay);
	NewPrimitive.SetPos(glm::vec2(-8.0f, 0.0f));
	
	NewPrimitive->SetVertices({
		glm::vec2(-1.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(-1.0f, 1.0f)
		});

	NewPrimitive->SetUVs({
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, 0.0f)
		});

	NewPrimitive->SetIndices({ 0, 1, 2, 2, 3, 0 });
	NewPrimitive->SetMaterial("Default");

	return NewPrimitive;
}

std::vector<FPrimitiveObject> SRenderSystem::DrawText(const std::string& Text, const glm::vec2& Origin, const FColorRGB& Color)
{
	std::vector<FPrimitiveObject> TextPrimitive(Text.length());

	glm::vec2 Offset(0.0f, 0.0f);
	int32_t i = 0;
	for (char C : Text)
	{
		FPrimitiveObject& NewPrimitive = TextPrimitive[i++];
		NewPrimitive.Create(Overlay);
		NewPrimitive.SetPos(Origin + Offset);
	
		const FSymbolQuad& Quad = Font->GetSymbolUV(C);

		NewPrimitive->SetVertices(Quad.Vertices);
		NewPrimitive->SetUVs(Quad.UVs);
		NewPrimitive->SetIndices({ 0, 1, 2, 2, 3, 0 });
		NewPrimitive->SetMaterial("FontAtlas");
		NewPrimitive->SetColor(Color);

		Offset.x += Quad.Advance;
	}

	return TextPrimitive;
}
