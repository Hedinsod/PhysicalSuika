#pragma once

#include "Graphics/Graphics.h"

class SOpenGLGraphics : public SGraphicsApi
{
public:
	SOpenGLGraphics();
	virtual ~SOpenGLGraphics();

	// Creating window & context
	virtual StdScoped<SGfxWindow> CreateGfxWindow(uint32_t InWidth, uint32_t InHeight, const std::string& InTitle) override;

	// Factories
	virtual StdScoped<SGfxShaderFactory> GetShaderFactory() override;
	virtual StdScoped<SGfxBufferFactory> GetBufferFactory() override;
	virtual StdScoped<SGfxTextureFactory> GetTextureFactory() override;

	// Render Commands
	virtual void DrawIndexed(uint32_t IndexCount) override;
	virtual void SetClearColor(const FColorRGB& InColor) override;
	virtual void Clear() override;

private:
	static void ProcessError(int32_t ErrorCode, const char* Description);

	void PostInit();

};

