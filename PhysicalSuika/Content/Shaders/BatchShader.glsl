#type vertex
#version 330 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_Texture;

uniform mat4 u_ViewProj;

out float v_Texture;
out vec4 v_Color;
out vec2 v_TexCoord;

void main()
{
	v_Texture = a_Texture;
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;

	gl_Position = u_ViewProj * a_Position;
}

#type pixel
#version 330 core

layout(location = 0) out vec4 Color;

in float v_Texture;
in vec2 v_TexCoord;
in vec4 v_Color;

uniform sampler2D u_Textures[8];

void main()
{
	Color = texture(u_Textures[int(v_Texture)], v_TexCoord) * v_Color;
}
