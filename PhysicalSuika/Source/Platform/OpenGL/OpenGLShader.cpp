#include "pch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>


SOpenGLShader::SOpenGLShader(const std::string& VertexSource, const std::string& FragmentSource)
{
	// Create an empty vertex shader handle
	GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Send the vertex shader Source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	const GLchar* Source = VertexSource.c_str();
	glShaderSource(VertexShader, 1, &Source, 0);

	// Compile the vertex shader
	glCompileShader(VertexShader);

	GLint IsCompiled = 0;
	glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &IsCompiled);
	if (IsCompiled == GL_FALSE)
	{
		GLint MaxLength = 0;
		glGetShaderiv(VertexShader, GL_INFO_LOG_LENGTH, &MaxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> InfoLog(MaxLength);
		glGetShaderInfoLog(VertexShader, MaxLength, &MaxLength, &InfoLog[0]);

		// We don't need the shader anymore.
		glDeleteShader(VertexShader);

		std::cerr << "SOpenGLShader(): Vertex Shader compilation failed:\n"
		          << InfoLog.data() << std::endl;
		
		GAssert(false);
		return;
	}

	// Create an empty fragment shader handle
	GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Send the fragment shader Source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	Source = FragmentSource.c_str();
	glShaderSource(FragmentShader, 1, &Source, 0);

	// Compile the fragment shader
	glCompileShader(FragmentShader);

	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &IsCompiled);
	if (IsCompiled == GL_FALSE)
	{
		GLint MaxLength = 0;
		glGetShaderiv(FragmentShader, GL_INFO_LOG_LENGTH, &MaxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> InfoLog(MaxLength);
		glGetShaderInfoLog(FragmentShader, MaxLength, &MaxLength, &InfoLog[0]);

		// We don't need the shader anymore.
		glDeleteShader(FragmentShader);
		// Either of them. Don't leak shaders.
		glDeleteShader(VertexShader);

		std::cerr << "SOpenGLShader(): Fragment Shader compilation failed:\n"
		          << InfoLog.data() << std::endl;

		// In this simple ShaderId, we'll just leave
		return;
	}

	// Vertex and fragment shaders are successfully compiled.
	// Now time to link them together into a ShaderId.
	// Get a ShaderId object.
	ShaderId = glCreateProgram();

	// Attach our shaders to our ShaderId
	glAttachShader(ShaderId, VertexShader);
	glAttachShader(ShaderId, FragmentShader);

	// Link our ShaderId
	glLinkProgram(ShaderId);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint IsLinked = 0;
	glGetProgramiv(ShaderId, GL_LINK_STATUS, (int*)&IsLinked);
	if (IsLinked == GL_FALSE)
	{
		GLint MaxLength = 0;
		glGetProgramiv(ShaderId, GL_INFO_LOG_LENGTH, &MaxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> InfoLog(MaxLength);
		glGetProgramInfoLog(ShaderId, MaxLength, &MaxLength, &InfoLog[0]);

		// We don't need the ShaderId anymore.
		glDeleteProgram(ShaderId);
		// Don't leak shaders either.
		glDeleteShader(VertexShader);
		glDeleteShader(FragmentShader);

		std::cerr << "SOpenGLShader(): Shader link failed:\n"
		          << InfoLog.data() << std::endl;

		// In this simple ShaderId, we'll just leave
		return;
	}

	// Always detach shaders after a successful link.
	glDetachShader(ShaderId, VertexShader);
	glDetachShader(ShaderId, FragmentShader);
}

SOpenGLShader::~SOpenGLShader()
{
	glDeleteProgram(ShaderId);
}

void SOpenGLShader::Bind()
{
	glUseProgram(ShaderId);
}

void SOpenGLShader::Unbind()
{
	glUseProgram(0);
}
