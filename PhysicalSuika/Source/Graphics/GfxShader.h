#pragma once

#include <string>
#include <glm/mat4x4.hpp>

class SGfxShader
{
public:
	virtual ~SGfxShader() = default;

	virtual void Bind() = 0;
	virtual void Unbind() = 0;

	virtual void UploadUniform(const char* ParameterName, const glm::mat4x4& mat) = 0;

};