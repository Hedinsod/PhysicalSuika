#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>


class SGfxShader
{
public:
	virtual ~SGfxShader() = default;

	virtual void Bind() = 0;
	virtual void Unbind() = 0;

	virtual void SetParameter(const char* ParameterName, const glm::mat4& mat) = 0;
	virtual void SetParameter(const char* ParameterName, const glm::vec4& vec) = 0;

};

enum class EGfxShaderType
{
	Vertex,
	Pixel,

	None
};

class SGfxShaderFactory
{
public:
	virtual ~SGfxShaderFactory();

	void AddSource(EGfxShaderType Type, const std::string& Source);
	void LoadSourceFromFile(const std::string& Path);
	
	virtual StdShared<SGfxShader> Build() = 0;

protected:
	std::vector<std::pair<EGfxShaderType, std::string>> Sources;

};
