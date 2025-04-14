#type vertex
#version 330 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Color;
uniform mat4 u_ViewProj;

out vec4 v_Color;

void main()
{
	v_Color = a_Color;

	gl_Position = u_ViewProj * a_Position;
}

#type pixel
#version 330 core

layout(location = 0) out vec4 Color;

in vec4 v_Color;

void main()
{
	Color = v_Color;
}
