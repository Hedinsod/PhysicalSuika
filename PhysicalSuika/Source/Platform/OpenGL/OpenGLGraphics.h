#pragma once

#include "Graphics/Graphics.h"

class SOpenGLGraphics : public SGraphicsApi
{
public:
	// Fabrics
	virtual StdScoped<SGfxShaderFactory> GetShaderFactory() override;

	virtual SGfxWindow* CreateGfxWindow(int InWidth, int InHeight, const std::string& InTitle) override;
	virtual StdShared<SGfxVertexBuffer> CreateVertexBuffer(const std::vector<float>& VertexData) override;
	virtual StdShared<SGfxIndexBuffer> CreateIndexBuffer(const std::vector<uint32_t>& IndexData) override;
	virtual StdShared<SGfxVertexData> CreateVertexData() override;

	// Render Commands
	virtual void DrawIndexed(const StdShared<SGfxVertexData>& VA) override;
	virtual void SetClearColor(const FColorRGB& InColor) override;
	virtual void Clear() override;

};

