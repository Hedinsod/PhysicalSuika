#pragma once

#include "Core/Utility.h"
#include "Physics/PhScene.h"
#include "Systems/Draw.h"
#include <memory>


class Engine final
{
public:
	static void Init();
	static SPhScene& GetCollision()
	{
		GAssert(Instance);
		return *Instance->Collision;
	}
	static SDraw& GetGraphics()
	{
		GAssert(Instance);
		return *Instance->Graphics;
	}

private:
	Engine() = default;
	static Engine* Instance;

	std::unique_ptr<SPhScene> Collision;
	std::unique_ptr<SDraw> Graphics;
};
