#pragma once

class CGeometry;
class SGfxShader;
class ACamera;
class CTransform;
struct FColorLinear;

class SRenderer
{
public:
	static void Init();

	static void Begin(const StdShared<ACamera>& Camera);
	static void Sumbit(const CGeometry* Geo, const CTransform& Trans, const FColorLinear& Color);

private: 
	static StdShared<SGfxShader> Shader;

};
