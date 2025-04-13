#include "pch.h"
#include "Graphics.h"
#include "Graphics/GfxShader.h"
#include "Graphics/GfxBuffers.h"

#include "Platform/OpenGL/OpenGLGraphics.h"


EGfxApi SGraphics::CurrentApi = EGfxApi::None;
SGraphicsApi* SGraphics::Api = nullptr;

void SGraphics::Init(EGfxApi InApi)
{
	CurrentApi = InApi;
	switch (CurrentApi)
	{
	case EGfxApi::OpenGL: Api = new SOpenGLGraphics; break;

	default: GAssert(false); break;
	}
}

SGraphicsApi::~SGraphicsApi()
{
}
