#pragma once

#include "Systems/Transform.h"
class CGeometry;
class AActor;

// FGeometryHandle - accessor to SGeometryData stored in the pool of the renderer
// It is just a handle, SGeometryData will live untill manually ereased 
class FGeometryHandle
{
public:
	FGeometryHandle(int32_t NewId = -1) : Id(NewId) { }

	static FGeometryHandle Create(AActor* Owner);
	void Erase();

	CGeometry& operator*();
	const CGeometry& operator*() const;
	CGeometry* operator->();
	const CGeometry* operator->() const;

private:
	int32_t Id;

};

// FPrimitiveObject is an object by itself
// It holds data (Transform) for actorless SGeometryData
// Essentially it behaves like unique pointer, it cannot be copyed 
// And can only be moved
class FPrimitiveObject
{
	friend class SRenderSystem;
public:
	FPrimitiveObject();

	FPrimitiveObject(FPrimitiveObject&& Other) noexcept;
	FPrimitiveObject& operator=(FPrimitiveObject&& Other) noexcept;

	// Non-copyable
	FPrimitiveObject(const FPrimitiveObject&) = delete;
	FPrimitiveObject& operator=(const FPrimitiveObject&) = delete;

	~FPrimitiveObject();

	void Erase();

	bool IsValid() { return Id >= 0; }

	void SetPos(const glm::vec2& InPos);

private:
	int32_t Id = -1;

	StdScoped<AActor> Dummy;

};