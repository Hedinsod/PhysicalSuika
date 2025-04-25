#pragma once

#include "Actor.h"

#include <queue>

class AFruit;

struct FFruitParents
{
	AFruit* ParentOne;
	AFruit* ParentTwo;
};


class AArbiter : public AActor
{
public:
	AArbiter(glm::vec2 InPos) : AActor(InPos) {}
	virtual ~AArbiter() = default;

	virtual void Tick(float DeltaTime) override;

	void AddTask(AFruit* ParentOne, AFruit* ParentTwo);

private:
	std::queue<FFruitParents> Tasks;

};
