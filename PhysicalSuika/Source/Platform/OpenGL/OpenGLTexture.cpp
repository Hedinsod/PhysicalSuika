#include "pch.h"
#include "OpenGLTexture.h"

#include <stb_image.h>
#include <glad/glad.h>


// ****************************************************************************
// ********** SOpenGLTexture **************************************************
// ****************************************************************************

SOpenGLTexture::SOpenGLTexture(uint32_t InWidth, uint32_t InHeight, uint32_t TargetFormat)
	: Width(InWidth)
	, Height(InHeight)
	, Format(TargetFormat)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &RenderId);
	glTextureStorage2D(RenderId, 1, (GLenum)Format, Width, Height);

	glTextureParameteri(RenderId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(RenderId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

SOpenGLTexture::~SOpenGLTexture()
{
	glDeleteTextures(1, &RenderId);
}

void SOpenGLTexture::UploadData(void* Data, uint32_t DataFormat)
{
	glTextureSubImage2D(RenderId, 0, 0, 0, Width, Height, (GLenum)DataFormat, GL_UNSIGNED_BYTE, Data);
}

void SOpenGLTexture::Bind(int32_t InSlot)
{
	Slot = InSlot;
	glBindTextureUnit(Slot, RenderId);
}

void SOpenGLTexture::Unbind()
{
	glBindTextureUnit(Slot, 0);
	Slot = -1;
}


// ****************************************************************************
// ********** SOpenGLTextureFactory *******************************************
// ****************************************************************************

StdShared<SGfxTexture> SOpenGLTextureFactory::LoadFromFile(const std::string& Path)
{
	int32_t TempWidth, TempHeight, TempChannels;

	stbi_set_flip_vertically_on_load(1);
	stbi_uc* Data = stbi_load(Path.c_str(), &TempWidth, &TempHeight, &TempChannels, 0);
	GAssert(Data);

	GLenum DataFormat = 0;
	if (TempChannels == 4)
	{
		DataFormat = GL_RGBA;
	}
	else if (TempChannels == 3)
	{
		DataFormat = GL_RGB;
	}

	// Target Format
	StdShared<SOpenGLTexture> NewTexture = MakeShared<SOpenGLTexture>(TempWidth, TempHeight, GL_RGBA8);
	NewTexture->UploadData(Data, DataFormat);

	stbi_image_free(Data);

	return NewTexture;
}

StdShared<SGfxTexture> SOpenGLTextureFactory::CreateFromData(void* Data, uint32_t Width, uint32_t Height, uint32_t DataFormat)
{
	StdShared<SOpenGLTexture> NewTexture = MakeShared<SOpenGLTexture>(Width, Height, GL_RGBA8);
	NewTexture->UploadData(Data, DataFormat);

	return NewTexture;
}
