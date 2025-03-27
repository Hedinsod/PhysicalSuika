#pragma once

#include "Graphics/GfxShader.h"

class SOpenGLShader : public SGfxShader
{
public:
	SOpenGLShader(const std::string& VertexSource, const std::string& FragmentSource);
	virtual ~SOpenGLShader();

	virtual void Bind() override;
	virtual void Unbind() override;

	virtual void UploadUniform(const char* ParameterName, const glm::mat4x4& mat) override;

private:
	uint32_t ShaderId;

};