#include "pch.h"

#include "RenderSystem.h"
#include "GeometryHandls.h"
#include "Systems/Engine.h"
#include "Core/Input.h"


SRenderSystem::SRenderSystem()
{
	Renderer = MakeScoped<SRenderer>();
}

void SRenderSystem::Tick()
{
	Renderer->Begin(CurrentCamera);
	Renderer->RenderPool(GeometryPool);
	Renderer->Finish();

	if (SInput::IsButtonPressed(EInputCode::Tab))
	{
		Renderer->Begin(CurrentCamera);
		Renderer->RenderPool(Overlay);
		Renderer->Finish();
	}
}

FPrimitiveObject SRenderSystem::DrawDot(const glm::vec2& Point, float Size)
{
	auto& Overlay = Engine::Renderer().Overlay;

	FPrimitiveObject NewPrimitive;
	NewPrimitive.Id = Overlay.Emplace(NewPrimitive.Dummy.get());
	NewPrimitive.SetPos(Point);

	glm::vec2 xShift(Size, 0.0f);
	glm::vec2 yShift(0.0f, Size);

	Overlay[NewPrimitive.Id].SetVertices({ glm::vec2(0.0f, 0.0f), xShift, yShift});
	Overlay[NewPrimitive.Id].SetIndices({ 0, 1, 2 });
	Overlay[NewPrimitive.Id].SetMaterial("Overlay");

	return NewPrimitive;
}

FPrimitiveObject SRenderSystem::DrawLine(const glm::vec2& Start, const glm::vec2& Finish, float Size)
{
	static AActor Dummy(glm::vec2(0.0f, 0.0f));
	Size /= 2;

	FPrimitiveObject NewPrimitive;
	NewPrimitive.Id = Overlay.Emplace(NewPrimitive.Dummy.get());

	glm::vec2 yShift(0.0f, Size);

	Overlay[NewPrimitive.Id].SetVertices({ Start - yShift, Finish - yShift, Finish + yShift, Start - yShift });
	Overlay[NewPrimitive.Id].SetIndices({ 0, 1, 2, 2, 3, 0 });
	Overlay[NewPrimitive.Id].SetMaterial("Overlay");

	return NewPrimitive;
}

void SRenderSystem::ErasePrimitive(FPrimitiveObject& Handle)
{
	Overlay.Remove(Handle.Id);
	Handle.Id = -1;
}
