#pragma once

#include "Core/Utility.h"
#include "GeometryComp.h"
#include "Core/Containers/SparseArray.h"
#include "GeometryHandle.h"


class AActor;
class ACamera;
class SGfxShader;
class SGfxVertexBuffer;
class SGfxIndexBuffer;
class SGfxVertexArray;


class SGeometryPool
{
	friend FGeometryHandle;

	struct FVertex
	{
		glm::vec4 Position;
		glm::vec4 Color;
	};

public:
	SGeometryPool();
	~SGeometryPool();

	FGeometryHandle CreateGeometry(AActor* InOwner);
	void RemoveGeometry(FGeometryHandle);

	void Begin(const StdShared<ACamera>& Camera);
	void Finish();

	void Tick();

private:
	TSparseArray<CGeometry> GeometryPool;

	StdShared<SGfxShader> Shader;
	StdShared<SGfxVertexBuffer> VBO;
	StdShared<SGfxIndexBuffer> IBO;
	StdShared<SGfxVertexArray> VAO;

	FVertex* VertexData;
	uint32_t NextVertex;

	uint32_t* IndexData;
	uint32_t NextIndex;

	uint32_t IndexOffset;

};
