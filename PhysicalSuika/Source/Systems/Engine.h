#pragma once

#include "Core/Utility.h"
#include "Physics/PhyScene.h"
#include "Renderer/GeometryPool.h"
#include "Systems/MaterialLibrary.h"
#include <memory>


class Engine final
{
public:
	static void Init();
	static void Shutdown();

	static SPhyScene& GetPhyScene()
	{
		GAssert(Instance);
		return *Instance->PhyScene;
	}
	static SGeometryPool& GetGraphics()
	{
		GAssert(Instance);
		return *Instance->Graphics;
	}

	static SMatirialLibrary& GetMaterialLibrary()
	{
		GAssert(Instance);
		return Instance->MatirialLibrary;
	}

private:
	Engine() = default;

	void LoadMaterials();

private:
	static Engine* Instance;

	StdScoped<SPhyScene> PhyScene;
	StdScoped<SGeometryPool> Graphics;
	
	SMatirialLibrary MatirialLibrary;

};
