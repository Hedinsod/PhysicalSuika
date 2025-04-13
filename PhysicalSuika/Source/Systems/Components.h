#pragma once

class AActor;

class CComponent
{
public:
	CComponent(AActor* InOwner)
		: Owner(InOwner)
	{
	}
	virtual ~CComponent() = default;

	const AActor& GetOwner() const { return *Owner; }
	AActor& GetOwner() { return *Owner; }

protected:
	// Weak pointer to owner
	AActor* Owner;

};
