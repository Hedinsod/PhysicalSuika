#pragma once

#include "Renderer.h"
#include "Font.h"
#include "Core/Containers/SparseArray.h"
#include "Core/SmartPointers.h"
#include "Systems/Transform.h"


// ****************************************************************************
// ********** SRenderSystem ***************************************************
// ****************************************************************************

class SRenderSystem
{
	friend class FGeometryHandle;
	friend class FPrimitiveObject;
public:
	SRenderSystem();
	
	void SetCamera(const StdShared<ACamera>& Camera) { CurrentCamera = Camera; }

	void Tick();

	// Primitives
	FPrimitiveObject DrawDot(const glm::vec2& Point, float Size);
	FPrimitiveObject DrawLine(const glm::vec2& Start, const glm::vec2& Finish, float Size);
	FPrimitiveObject DrawBox(const glm::vec2& LeftUpper, const glm::vec2& RightLower);

	std::vector<FPrimitiveObject> DrawText(const std::string& Text, const glm::vec2& Origin, const FColorRGB& Color);

private:
	TSparseArray<CGeometry> GeometryPool;
	TSparseArray<CGeometry> Overlay;
	TSparseArray<CGeometry> DebugOverlay;

	StdScoped<SRenderer> Renderer;
	StdScoped<SFont> Font;
	
	StdShared<ACamera> CurrentCamera;

};


