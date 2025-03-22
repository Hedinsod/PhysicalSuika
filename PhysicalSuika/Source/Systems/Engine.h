#pragma once

#include "Core/Utility.h"
#include "Systems/Collision.h"
#include "Systems/Graphics.h"
#include <memory>


class Engine final
{
public:
	static void Init(SGfxContext* InContext);
	static SCollisionCheck& GetCollision()
	{
		GAssert(Instance);
		return *Instance->Collision;
	}
	static SGraphics& GetGraphics()
	{
		GAssert(Instance);
		return *Instance->Graphics;
	}

private:
	Engine() = default;
	static Engine* Instance;

	std::unique_ptr<SCollisionCheck> Collision;
	std::unique_ptr<SGraphics> Graphics;
};
