#include "pch.h"
#include "Transform.h"


CTransform::CTransform(AActor* InOwner, float InX, float InY)
	: Owner(InOwner)
	, Pos({ InX, InY })
	, Rot({ 1.f, 0.f })
{

}

void CTransform::SetRotation(float Angle)
{
	Rot.x = std::cos(Angle);
	Rot.y = std::sin(Angle);
}
