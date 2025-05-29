#include "pch.h"
#include "Arbiter.h"

#include "Fruit.h"
#include "Hand.h"
#include "Game.h"

#include "Systems/Engine.h"
#include "Renderer/GeometryComp.h"


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

	virtual bool Execute(AArbiter& TheArbiter) override
	{
		TheArbiter.GetGame()->AddEntity<AFruit>(SpawnPoint, SpawnType);
		return true;
	}
};

struct FPopFruitsTask : FArbiterTask
{
	explicit FPopFruitsTask(std::vector<AFruit*>&& FruitsToPop)
		: Victims(std::move(FruitsToPop))
		, Index(0)
	{
		GAssert(Victims.size() > 0);
	}

	std::vector<AFruit*> Victims;
	int32_t Index;

	virtual bool Execute(AArbiter& TheArbiter) override
	{
		Victims[Index++]->Delete(); // TODO: pop!
		return Index == Victims.size();
	}
};

// ****************************************************************************
// ********** AArbiter ********************************************************
// ****************************************************************************

AArbiter::AArbiter(glm::vec2 InPos)
	: AActor(InPos)
{
	Engine::GetPhyScene().SetOnCollisionEventHandler(std::bind(&AArbiter::Merge, this, std::placeholders::_1, std::placeholders::_2));

	TitleLabel = Engine::Renderer().DrawText("Suika", glm::vec2(-10.5f, 16.0f), TextColor);
	ScoreLabel = Engine::Renderer().DrawText("Score: ", glm::vec2(6.0f, 15.0f), TextColor);
	ScoreValue = Engine::Renderer().DrawText("0", glm::vec2(10.0f, 15.0f), TextColor);
}

AArbiter::~AArbiter()
{
	TitleLabel.clear();
	ScoreLabel.clear();
	ScoreValue.clear();
}

void AArbiter::Tick(float DeltaTime)
{
	for (uint32_t Idx = 0; Idx < Tasks.size(); Idx++)
	{
		const bool bResult = Tasks[Idx]->Execute(*this);

		if (bResult)
		{
			std::swap(Tasks[Idx], Tasks[Tasks.size() - 1]);
			Tasks.erase(Tasks.end()-1);
		}
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


		TEnum<EFruitType> ChildType = RhsFruit->GetType() + 1;
		Score += ChildType.ToInteger() * 10; // 10 points per new fruit level

		AddTask(new FAddFruitTask(SpawnPoint, ChildType.ToEnum()));

		ScoreValue = Engine::Renderer().DrawText(std::to_string(Score), glm::vec2(10.0f, 15.0f), TextColor);
	}
}

void AArbiter::Finish()
{
	// Disable Hand
	Hand.lock()->Hold();

	// Freeze
	std::vector<AFruit*> AllFruits = GetGame()->GetFruits();
	for (AFruit* Fruit : AllFruits)
	{
		Fruit->Hold();
	}

	// And pop!
	FPopFruitsTask* PopAll = new FPopFruitsTask(GetGame()->GetFruits());
	AddTask(PopAll);
}

void AArbiter::AddTask(FArbiterTask* InTask)
{
	Tasks.push_back(InTask);
}
