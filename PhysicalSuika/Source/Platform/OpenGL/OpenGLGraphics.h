#pragma once

#include "Graphics/Graphics.h"

class SOpenGLGraphics : public SGraphicsApi
{
public:
	SOpenGLGraphics();
	
	// Fabrics
	virtual SGfxWindow* CreateGfxWindow(int InWidth, int InHeight, const std::string& InTitle) override;

	virtual StdScoped<SGfxShaderFactory> GetShaderFactory() override;
	virtual StdScoped<SGfxBufferFactory> GetBufferFactory() override;



	// Render Commands
	virtual void DrawIndexed(uint32_t IndexCount) override;
	virtual void SetClearColor(const FColorRGB& InColor) override;
	virtual void Clear() override;

};

