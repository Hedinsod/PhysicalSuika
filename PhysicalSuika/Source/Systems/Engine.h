#pragma once

#include "Core/Utility.h"
#include "Physics/PhyScene.h"
#include "Systems/Draw.h"
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

private:
	Engine() = default;
	static Engine* Instance;

	std::unique_ptr<SPhyScene> PhyScene;
	std::unique_ptr<SDraw> Graphics;
};
