#type vertex
#version 330 core

layout(location = 0) in vec4 a_Position;
uniform mat4 u_ViewProj;

void main()
{
	gl_Position = u_ViewProj * a_Position;
}

#type pixel
#version 330 core

layout(location = 0) out vec4 Color;

void main()
{
	Color = vec4(0.0, 1.0, 0.0, 1.0);
}
