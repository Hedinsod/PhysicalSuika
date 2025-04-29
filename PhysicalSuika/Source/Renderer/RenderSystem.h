#pragma once

#include "Renderer.h"
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
	void ErasePrimitive(FPrimitiveObject& Handle);

private:
	TSparseArray<CGeometry> GeometryPool;
	TSparseArray<CGeometry> Overlay;

	StdScoped<SRenderer> Renderer;

	StdShared<ACamera> CurrentCamera;

};


