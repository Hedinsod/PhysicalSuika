#pragma once

#include <cstdint>
class CGeometry;

struct FGeometryHandle
{
	uint32_t Id;

	CGeometry& operator*();
	const CGeometry& operator*() const;
	CGeometry& operator->();
	const CGeometry& operator->() const;
};
