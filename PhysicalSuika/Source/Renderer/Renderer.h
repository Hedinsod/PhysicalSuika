#pragma once

#include "Core/Utility.h"
#include "GeometryComp.h"
#include "Core/Containers/SparseArray.h"


class AActor;
class ACamera;
class SGfxShader;
class SGfxVertexBuffer;
class SGfxIndexBuffer;
class SGfxVertexArray;
class SGfxTexture;


// ****************************************************************************
// ********** SRenderer *******************************************************
// ****************************************************************************

class SRenderer
{
	struct FVertex
	{
		glm::vec4 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexSlot;
	};

public:
	SRenderer();
	~SRenderer();

	void Begin(const StdShared<ACamera>& Camera);
	void RenderPool(TSparseArray<CGeometry>& Pool);
	void Finish();

private:
	StdShared<SGfxShader> Shader;
	StdShared<SGfxVertexBuffer> VBO;
	StdShared<SGfxIndexBuffer> IBO;
	StdShared<SGfxVertexArray> VAO;

	FVertex* VertexData;
	uint32_t NextVertex;

	uint32_t* IndexData;
	uint32_t NextIndex;

	uint32_t IndexOffset;

	uint32_t NextTexSlot = 0;

	std::unordered_map<StdShared<SGfxTexture>, float> TextureToSlot;

};
