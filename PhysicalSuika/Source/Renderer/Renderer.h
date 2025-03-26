#pragma once


class CGeometry;
class SGfxShader;

class SRenderer
{
public:
	static void Init();
	static void Sumbit(CGeometry* Geo);

private: 
	static std::shared_ptr<SGfxShader> Shader;

	static const std::string BasicVertexShader;
	static const std::string BasicPixelShader;


};
