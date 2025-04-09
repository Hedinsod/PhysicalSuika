#include "pch.h"
#include "Camera.h"
#include "Core/Application.h"
#include "Core/Input.h"

ACamera::ACamera(glm::vec2 InPos, float InAspectRatio, float InZoom)
	: AActor(InPos)
	, AspectRatio(InAspectRatio)
	, Zoom(InZoom)
{
	GApp->AddOnResizeEventHandler(std::bind(&ACamera::OnResize, this, std::placeholders::_1, std::placeholders::_2));
}

ACamera::~ACamera()
{
	// Unsubscribe !
	// GApp->SetOnResizeEventHandler(nullptr);
}

void ACamera::OnResize(int32_t NewWidth, int32_t NewHeight)
{
	AspectRatio = (float)NewWidth / NewHeight;

	

	UpdateProjView();
}

void ACamera::UpdateProjView()
{
	float Left = -AspectRatio * Zoom;
	float Right = AspectRatio * Zoom;
	float Top = Zoom;
	float Bottom = -Zoom;
	Projection = glm::ortho(Left, Right, Bottom, Top);

	ProjView = Projection * glm::inverse(Trans.GetModel());
}
