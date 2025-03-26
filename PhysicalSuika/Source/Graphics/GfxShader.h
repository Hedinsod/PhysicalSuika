#pragma once

#include <string>

class SGfxShader
{
public:
	//SGfxShader(const std::string& VertexSource, const std::string& FragmentSource) = 0;
	virtual ~SGfxShader() = default;

	virtual void Bind() = 0;
	virtual void Unbind() = 0;

};