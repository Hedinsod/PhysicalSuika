#pragma once

#include "Graphics/Graphics.h"

#include <glm/glm.hpp>

class SWinApiGraphics : public SGraphicsApi
{
public:
	virtual SGfxWindow* CreateGfxWindow(int InWidth, int InHeight, const std::string& InTitle) override;
	virtual SGfxVertexDataPtr CreateVertexData() override;

	// Fake
	virtual SGfxShaderPtr CreateShader(const std::string& VertexSource, const std::string& FragmentSource) override;
	virtual SGfxVertexBufferPtr CreateVertexBuffer(const std::vector<float>& VertexData) override;
	virtual SGfxIndexBufferPtr CreateIndexBuffer(const std::vector<uint32_t>& IndexData) override;

	// Render Commands
	virtual void DrawIndexed(const SGfxVertexDataPtr& VA) override;
	virtual void SetClearColor(const FColorRGB& InColor) override { ClearColor = InColor; }
	virtual void Clear() override { FillRect({ 0, 0 }, { 800, 600 }, ClearColor); }

private:
	HWND hWnd = 0;
	FColorRGB ClearColor;

	void FillRect(glm::vec2 Pivot, glm::vec2 Size, FColorRGB Color);
	void DrawLine(glm::vec2 Start, glm::vec2 End, FColorRGB Color);
};