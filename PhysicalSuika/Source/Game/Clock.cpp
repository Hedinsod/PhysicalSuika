#include "pch.h"

#include "Systems/Engine.h"
#include "Game/Game.h"
#include "Game/Clock.h"


EHand::EHand(EClock* Clock, float InX, float InY, float InLength)
	: Entity(InX, InY)
	, Parent(Clock)
	, Length(InLength)
	, Progress(0.0)
{
	Hand = Engine::GetGraphics().CreateGeometry(this);
	FColor HandColor{ 0, 200, 0 };
	Hand->AddPoint({ 0, 0 }, HandColor );
	Hand->AddPoint({ 0, -Length }, HandColor );
}

EHand::~EHand()
{
	Engine::GetGraphics().RemoveGeometry(Hand);
	delete Hand;
}

void EHand::Tick()
{
	float Angle = Progress * 2 * static_cast<float>(M_PI);

	Trans = Parent->GetTransform();
	Trans.SetRotation(Angle);
}


EClock::EClock(float InX, float InY, float InScale)
	: Entity(InX, InY)
	, Scale(InScale)
{
	Length = BaseLength * Scale;

	Collider = new CollisionBox(this, +Length, -Length, +Length, -Length);
	Engine::GetCollision().AddCollisionBox(Collider);

	Box = Engine::GetGraphics().CreateGeometry(this);

	FColor BoxColor{ 0, 128, 128 };
	Box->AddPoint({ -Length, -Length }, BoxColor);
	Box->AddPoint({ -Length,  Length }, BoxColor);
	Box->AddPoint({  Length,  Length }, BoxColor);
	Box->AddPoint({  Length, -Length }, BoxColor);
	Box->AddPoint({ -Length, -Length }, BoxColor);  // for cycling

	HourHand = new EHand(this, Trans.Pos.x, Trans.Pos.y, Length * 0.8f);
	MinHand = new EHand(this, Trans.Pos.x, Trans.Pos.y, Length);
	SecHand = new EHand(this, Trans.Pos.x, Trans.Pos.y, Length);
}

EClock::~EClock()
{
	Engine::GetCollision().RemoveCollisionBox(Collider);
	delete Collider;

	Engine::GetGraphics().RemoveGeometry(Box);
	delete Box;

	delete HourHand;
	delete MinHand;
	delete SecHand;
}

void EClock::Tick()
{
	int hr, min, sec;
	Utility::GetTime(hr, min, sec);

	HourHand->SetProgress(hr / 24.f);
	MinHand->SetProgress(min / 60.f);
	SecHand->SetProgress(sec / 60.f);

	HourHand->Tick();
	MinHand->Tick();
	SecHand->Tick();

}
