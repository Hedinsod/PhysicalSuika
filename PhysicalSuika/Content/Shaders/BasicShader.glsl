#type vertex
#version 330 core

layout(location = 0) in vec2 a_Position;

uniform mat4 u_ViewProj;
uniform mat4 u_Model;

void main()
{
	gl_Position = u_ViewProj * u_Model * vec4(a_Position, 0.0, 1.0);
}

#type pixel
#version 330 core

layout(location = 0) out vec4 Color;
uniform vec4 u_Color;
				
void main()
{
	Color = u_Color;
}
