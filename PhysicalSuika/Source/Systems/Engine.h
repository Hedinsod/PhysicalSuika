#pragma once

#include "Core/Utility.h"
#include "Physics/PhyScene.h"
#include "Renderer/Draw.h"
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
	static SDraw& GetGraphics()
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

	std::unique_ptr<SPhyScene> PhyScene;
	std::unique_ptr<SDraw> Graphics;
	
	SMatirialLibrary MatirialLibrary;

};
