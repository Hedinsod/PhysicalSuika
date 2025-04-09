#pragma once

#include "Graphics/Graphics.h"

class SOpenGLGraphics : public SGraphicsApi
{
public:
	// Fabrics
	virtual SGfxWindow* CreateGfxWindow(int InWidth, int InHeight, const std::string& InTitle) override;
	virtual std::shared_ptr<SGfxShader> CreateShader(const std::string& VertexSource, const std::string& FragmentSource) override;
	virtual std::shared_ptr<SGfxVertexBuffer> CreateVertexBuffer(const std::vector<float>& VertexData) override;
	virtual std::shared_ptr<SGfxIndexBuffer> CreateIndexBuffer(const std::vector<uint32_t>& IndexData) override;
	virtual std::shared_ptr<SGfxVertexData> CreateVertexData() override;

	// Render Commands
	virtual void DrawIndexed(const std::shared_ptr<SGfxVertexData>& VA) override;
	virtual void SetClearColor(const FColorRGB& InColor) override;
	virtual void Clear() override;

};

