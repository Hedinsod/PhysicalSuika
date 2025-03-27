#pragma once

class AActor;

class CComponent
{
public:
	CComponent(AActor* InOwner)
		: Owner(InOwner)
	{
	}

	const AActor& GetOwner() const { return *Owner; }
	AActor& GetOwner() { return *Owner; }

protected:
	AActor* Owner;

};
