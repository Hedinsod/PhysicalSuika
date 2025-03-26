#pragma once

#include "Graphics/Graphics.h"

class SOpenGLGraphics : public SGraphicsApi
{
public:
	// Fabrics
	virtual SGfxWindow* CreateGfxWindow(int InWidth, int InHeight, const std::string& InTitle) override;
	virtual SGfxShaderPtr CreateShader(const std::string& VertexSource, const std::string& FragmentSource) override;
	virtual SGfxVertexBufferPtr CreateVertexBuffer(const std::vector<float>& VertexData) override;
	virtual SGfxIndexBufferPtr CreateIndexBuffer(const std::vector<uint32_t>& IndexData) override;
	virtual SGfxVertexDataPtr CreateVertexData() override;

	// Render Commands
	virtual void DrawIndexed(const SGfxVertexDataPtr& VA) override;
	virtual void SetClearColor(const FColor& InColor) override;
	virtual void Clear() override;

};

