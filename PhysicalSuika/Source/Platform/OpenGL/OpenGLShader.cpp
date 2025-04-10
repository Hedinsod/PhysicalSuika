#include "pch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

// ****************************************************************************
// **** SOpenGLShaderFabric ***************************************************
// ****************************************************************************

SOpenGLShader::SOpenGLShader(uint32_t InRenderId)
	: RenderId(InRenderId)
{
}

SOpenGLShader::~SOpenGLShader()
{
	glDeleteProgram(RenderId);
}

void SOpenGLShader::Bind()
{
	glUseProgram(RenderId);
}

void SOpenGLShader::Unbind()
{
	glUseProgram(0);
}

void SOpenGLShader::SetParameter(const char* ParameterName, const glm::mat4& mat)
{
	GLint Loc = glGetUniformLocation(RenderId, ParameterName);
	glUniformMatrix4fv(Loc, 1, GL_FALSE, glm::value_ptr(mat));
}

void SOpenGLShader::SetParameter(const char* ParameterName, const glm::vec4& vec)
{
	GLint Loc = glGetUniformLocation(RenderId, ParameterName);
	glUniform4f(Loc, vec.r, vec.g, vec.b, vec.a);
}

// ****************************************************************************
// **** SOpenGLShaderFabric ***************************************************
// ****************************************************************************

StdShared<SGfxShader> SOpenGLShaderFactory::Build()
{
	GLuint NewProgram = glCreateProgram();

	//////////////////
	// Compile 
	std::vector<GLuint> ShaderIds;
	ShaderIds.reserve(Sources.size());
	for (auto& [Type, Source] : Sources)
	{
		GLuint Shader;
		switch (Type)
		{
		case EGfxShaderType::Vertex: Shader = glCreateShader(GL_VERTEX_SHADER); break;
		case EGfxShaderType::Pixel: Shader = glCreateShader(GL_FRAGMENT_SHADER); break;
		default: GAssert(false);
		}

		const GLchar* SourceCStr = Source.c_str();
		glShaderSource(Shader, 1, &SourceCStr, 0);

		glCompileShader(Shader);

		GLint IsCompiled = 0;
		glGetShaderiv(Shader, GL_COMPILE_STATUS, &IsCompiled);
		if (IsCompiled == GL_FALSE)
		{
			GLint MaxLength = 0;
			glGetShaderiv(Shader, GL_INFO_LOG_LENGTH, &MaxLength);

			std::vector<GLchar> InfoLog(MaxLength);
			glGetShaderInfoLog(Shader, MaxLength, &MaxLength, &InfoLog[0]);

			glDeleteShader(Shader);

			Utility::Log("Shader compilation failed.\n" + std::string(InfoLog.data()));
			continue;
		}

		glAttachShader(NewProgram, Shader);
		ShaderIds.push_back(Shader);
	}

	//////////////////
	// Link
	glLinkProgram(NewProgram);

	GLint IsLinked = 0;
	glGetProgramiv(NewProgram, GL_LINK_STATUS, (GLint*)&IsLinked);
	if (IsLinked == GL_FALSE)
	{
		GLint MaxLength = 0;
		glGetProgramiv(NewProgram, GL_INFO_LOG_LENGTH, &MaxLength);

		std::vector<GLchar> InfoLog(MaxLength);
		glGetProgramInfoLog(NewProgram, MaxLength, &MaxLength, &InfoLog[0]);

		glDeleteProgram(NewProgram);
		for (GLuint Shader : ShaderIds)
			glDeleteShader(Shader);

		Utility::Log("Shader link failed.\n" + std::string(InfoLog.data()));
		return nullptr;
	}

	for (GLuint Shader : ShaderIds)
		glDetachShader(NewProgram, Shader);

	return MakeShared<SOpenGLShader>(NewProgram);
}
