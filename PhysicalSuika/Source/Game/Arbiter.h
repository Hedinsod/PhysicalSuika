#pragma once

#include "Actor.h"
#include "Graphics/Types.h"
#include "Renderer/GeometryHandls.h"

#include <queue>

class AFruit;
class AHand;
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

	void SetHand(StdWeak<AHand>& InHand) { Hand = InHand; }


private:
	static inline const FColorRGB TextColor = FColorRGB(225, 175, 0);

	std::vector<FPrimitiveObject> TitleLabel;
	std::vector<FPrimitiveObject> ScoreLabel;
	std::vector<FPrimitiveObject> ScoreValue;

	std::vector<FArbiterTask*> Tasks;

	int32_t Score = 0;

	StdWeak<AHand> Hand;
};
