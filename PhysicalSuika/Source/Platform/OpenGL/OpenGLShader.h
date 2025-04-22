#pragma once

#include "Graphics/GfxShader.h"


class SOpenGLShader : public SGfxShader
{
public:
	explicit SOpenGLShader(uint32_t InRenderId);
	virtual ~SOpenGLShader();

	virtual void Bind() override;
	virtual void Unbind() override;

	virtual void SetParameter(const char* ParameterName, const glm::mat4& mat) override;
	virtual void SetParameter(const char* ParameterName, const glm::vec4& vec) override;
	virtual void SetParameter(const char* Name, const int32_t* vs, const uint32_t Count) override;

private:
	uint32_t RenderId;

};

class SOpenGLShaderFactory : public SGfxShaderFactory
{
public:
	virtual StdShared<SGfxShader> Build() override;

};
