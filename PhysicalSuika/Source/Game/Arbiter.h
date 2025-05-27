#pragma once

#include "Actor.h"
#include "Renderer/GeometryHandls.h"

#include <queue>

class AFruit;
class AArbiter;

struct FArbiterTask
{
	virtual ~FArbiterTask() = default;
	virtual bool Execute(AArbiter& TheArbiter) = 0;
};

class AArbiter : public AActor
{
public:
	AArbiter(glm::vec2 InPos);
	virtual ~AArbiter() override;

	virtual void Tick(float DeltaTime) override;

	void Merge(AActor* ParentOne, AActor* ParentTwo);
	void Finish();

	void AddTask(FArbiterTask* InTask);

private:
	std::vector<FPrimitiveObject> TitleLabel;
	std::vector<FPrimitiveObject> ScoreLabel;

	std::vector<FArbiterTask*> Tasks;

	int32_t Score = 0;

};
