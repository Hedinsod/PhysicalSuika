#pragma once

#include "Core/Utility.h"
#include "Physics/PhyScene.h"
#include "Renderer/Renderer.h"
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
	static SRenderer& Renderer()
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
	StdScoped<SRenderer> Graphics;
	
	SMatirialLibrary MatirialLibrary;

};
