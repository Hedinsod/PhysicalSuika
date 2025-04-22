#pragma once

#include "Core/SmartPointers.h"

class SGfxTexture
{
public:
	virtual ~SGfxTexture() = default;

	virtual void Bind(int32_t Slot) = 0;
	virtual void Unbind() = 0;

};


class SGfxTextureFactory
{
public:
	virtual StdShared<SGfxTexture> LoadFromFile(const std::string& Path) = 0;
	virtual StdShared<SGfxTexture> CreateFromData(void* Data, uint32_t Width, uint32_t Height, uint32_t DataFormat) = 0;

private:

};

