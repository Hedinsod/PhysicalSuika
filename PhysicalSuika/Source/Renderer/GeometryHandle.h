#pragma once

#include <cstdint>
class CGeometry;

struct FGeometryHandle
{
	uint32_t Id;

	CGeometry& operator*();
	const CGeometry& operator*() const;
	CGeometry* operator->();
	const CGeometry* operator->() const;
};

struct FPrimitiveHandle
{
	int32_t Id = -1;

	FPrimitiveHandle() = default;
	explicit FPrimitiveHandle(int32_t NewId) : Id(NewId) { }
		
	FPrimitiveHandle(FPrimitiveHandle&& Other) noexcept;
	FPrimitiveHandle(const FPrimitiveHandle& Other) = delete;

	FPrimitiveHandle& operator =(FPrimitiveHandle&& Other) noexcept;
	FPrimitiveHandle& operator =(const FPrimitiveHandle& Other) = delete;

	~FPrimitiveHandle();

	bool IsValid() const { return Id != -1; }
};
