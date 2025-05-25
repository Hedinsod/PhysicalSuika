#include "pch.h"
#include "Arbiter.h"

#include "Fruit.h"
#include "Game.h"

#include "Systems/Engine.h"


// ****************************************************************************
// ********** FAddFruitTask ***************************************************
// ****************************************************************************

struct FAddFruitTask : FArbiterTask
{
	FAddFruitTask(glm::vec2 InSpawnPoint, EFruitType InSpawnType)
		: SpawnPoint(InSpawnPoint)
		, SpawnType(InSpawnType)
	{ }

	glm::vec2 SpawnPoint;
	EFruitType SpawnType;

	virtual void Execute(AArbiter& TheArbiter) override
	{
		TheArbiter.GetGame()->AddEntity<AFruit>(SpawnPoint, SpawnType);
	}
};

// ****************************************************************************
// ********** AArbiter ********************************************************
// ****************************************************************************

AArbiter::AArbiter(glm::vec2 InPos)
	: AActor(InPos)
{
	Engine::GetPhyScene().SetOnCollisionEventHandler(std::bind(&AArbiter::Merge, this, std::placeholders::_1, std::placeholders::_2));
}

void AArbiter::Tick(float DeltaTime)
{
	while (!Tasks.empty())
	{
		FArbiterTask* Task = Tasks.front();
		Tasks.pop();

		Task->Execute(*this);
		delete Task;
	}
}

void AArbiter::Merge(AActor* InLeft, AActor* InRight)
{
	AFruit* LhsFruit = dynamic_cast<AFruit*>(InLeft);
	AFruit* RhsFruit = dynamic_cast<AFruit*>(InRight);
	
	if (!LhsFruit || !RhsFruit)
		return;

	if (LhsFruit->GetType() == RhsFruit->GetType() && RhsFruit->GetType() != EFruitType::Watermelon)
	{
		LhsFruit->Delete();
		RhsFruit->Delete();

		const CTransform& LhsTrans = LhsFruit->GetTransform();
		const CTransform& RhsTrans = RhsFruit->GetTransform();

		glm::vec2 SpawnPoint = (LhsTrans.GetPos() + RhsTrans.GetPos()) * 0.5f;
		EFruitType ChildType = static_cast<EFruitType>(static_cast<int16_t>(RhsFruit->GetType()) + 1);

		Tasks.emplace(new FAddFruitTask(SpawnPoint, ChildType));
	}
}
