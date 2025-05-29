#include "pch.h"
#include "Glass.h"
#include "Systems/Engine.h"
#include "Renderer/GeometryComp.h"


static const struct FGlassData
{
	float HalfWidth = 5.0f;
	float HalfHeight = 8.0f;

	// Thickness of the collision boxes of the walls
	float HalfThick = 0.5f;
	
} GlassData;


AGlass::AGlass(glm::vec2 InPos)
	: AActor(InPos)
{
	std::vector<glm::vec2> Points = { 
			glm::vec2(-GlassData.HalfWidth, 2 * GlassData.HalfHeight),
			glm::vec2(-GlassData.HalfWidth, 0.0f),
			glm::vec2(GlassData.HalfWidth, 0.0f),
			glm::vec2(GlassData.HalfWidth, 2 * GlassData.HalfHeight)
		};
		
	GeoHandle = FGeometryHandle::Create(this);
	GeoHandle->SetVertices(Points);
	GeoHandle->SetIndices({ 0, 1, 3, 1, 2, 3 });
	GeoHandle->SetMaterial("Glass");
	
	FColliderShape* LeftBox = FColliderShape::Create<FBoxCollider>(
		{ -(GlassData.HalfWidth + GlassData.HalfThick), GlassData.HalfHeight }, // Pivot position
		-GlassData.HalfThick,     // Left
		 GlassData.HalfHeight,    // Top
		 GlassData.HalfThick,     // Right
		-GlassData.HalfHeight     // Bottom
	);
	FColliderShape* RightBox = FColliderShape::Create<FBoxCollider>(
		{ GlassData.HalfWidth + GlassData.HalfThick, GlassData.HalfHeight }, // Pivot position
		-GlassData.HalfThick,     // Left
		 GlassData.HalfHeight,    // Top
		 GlassData.HalfThick,     // Right
		-GlassData.HalfHeight     // Bottom
	);
	FColliderShape* BottomBox = FColliderShape::Create<FBoxCollider>(
		{ 0.f, -GlassData.HalfThick }, // Pivot position
		-GlassData.HalfWidth,     // Left
		 GlassData.HalfThick,     // Top
		 GlassData.HalfWidth,     // Right
		-GlassData.HalfThick      // Bottom
	);

	Left = Engine::GetPhyScene().CreateRigidBody(this, "Glass", LeftBox);
	Right = Engine::GetPhyScene().CreateRigidBody(this, "Glass", RightBox);
	Bottom = Engine::GetPhyScene().CreateRigidBody(this, "Glass", BottomBox);
}

AGlass::~AGlass()
{
	GeoHandle.Erase();

	Engine::GetPhyScene().RemoveRigidBody(Left);
	Engine::GetPhyScene().RemoveRigidBody(Right);
	Engine::GetPhyScene().RemoveRigidBody(Bottom);
}
