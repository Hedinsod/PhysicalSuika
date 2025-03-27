#pragma once


class CGeometry;
class SGfxShader;
class ACamera;
class CTransform;

class SRenderer
{
public:
	static void Init();

	static void Begin(const std::shared_ptr<ACamera>& Camera);
	static void Sumbit(const CGeometry* Geo, const CTransform& Trans);

private: 
	static std::shared_ptr<SGfxShader> Shader;

	static const std::string BasicVertexShader;
	static const std::string BasicPixelShader;


};
