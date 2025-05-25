#pragma once

#include "Actor.h"

#include <queue>

class AFruit;
class AArbiter;

struct FArbiterTask
{
	virtual ~FArbiterTask() = default;
	virtual void Execute(AArbiter& TheArbiter) = 0;
};

class AArbiter : public AActor
{
public:
	AArbiter(glm::vec2 InPos);
	virtual ~AArbiter() = default;

	virtual void Tick(float DeltaTime) override;

	void Merge(AActor* ParentOne, AActor* ParentTwo);


private:
	std::queue<FArbiterTask*> Tasks;

	int32_t Score = 0;

};
