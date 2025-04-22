#pragma once

#include "Graphics/GfxTexture.h"


// ****************************************************************************
// ********** SOpenGLTexture **************************************************
// ****************************************************************************

class SOpenGLTexture : public SGfxTexture
{
public:
	SOpenGLTexture(uint32_t Width, uint32_t Height, uint32_t TargetFormat);
	virtual ~SOpenGLTexture();

	void UploadData(void* Data, uint32_t DataFormat);

	virtual void Bind(int32_t Slot) override;
	virtual void Unbind() override;

private:
	uint32_t RenderId;

	uint32_t Width;
	uint32_t Height;
	uint32_t Format;

	// Video adapter texture slot
	int32_t Slot = 0;

};


// ****************************************************************************
// ********** SOpenGLTextureFactory *******************************************
// ****************************************************************************

class SOpenGLTextureFactory : public SGfxTextureFactory
{
public:
	virtual StdShared<SGfxTexture> LoadFromFile(const std::string& Path) override;
	virtual StdShared<SGfxTexture> CreateFromData(void* Data, uint32_t Width, uint32_t Height, uint32_t DataFormat) override;

private:

};


