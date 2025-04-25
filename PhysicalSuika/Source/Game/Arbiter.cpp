#include "pch.h"
#include "Arbiter.h"

#include "Fruit.h"
#include "Game.h"

void AArbiter::Tick(float DeltaTime)
{
	while (!Tasks.empty())
	{
		FFruitParents Task = Tasks.front();
		Tasks.pop();

		GAssert(Task.ParentOne->GetType() != EFruitType::Watermelon);

		EFruitType ChildType = static_cast<EFruitType>(static_cast<int16_t>(Task.ParentOne->GetType()) + 1);

		const CTransform& TransOne = Task.ParentOne->GetTransform();
		const CTransform& TransTwo = Task.ParentTwo->GetTransform();

		glm::vec2 SpawnPoint = (TransOne.GetPos() + TransTwo.GetPos()) * 0.5f;

		Task.ParentOne->Delete();
		Task.ParentTwo->Delete();
		GetGame()->AddEntity<AFruit>(SpawnPoint, ChildType);
	}
	
}

void AArbiter::AddTask(AFruit* InParentOne, AFruit* InParentTwo)
{
	Tasks.emplace(InParentOne, InParentTwo);

}
