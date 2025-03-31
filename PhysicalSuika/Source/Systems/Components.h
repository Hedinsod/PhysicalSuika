#pragma once

class AActor;

class CComponent
{
public:
	CComponent(AActor* InOwner)
		: Owner(InOwner)
	{
	}
	CComponent(const CComponent&) = default;
	CComponent(CComponent&&) = default;
	virtual ~CComponent() = default;


	const AActor& GetOwner() const { return *Owner; }
	AActor& GetOwner() { return *Owner; }

protected:
	AActor* Owner;

};
