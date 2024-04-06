#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in float textureID;

uniform mat4 u_MVP;

out float v_textureID;
out vec2 v_TexCoord;

void main()
{
	gl_Position = u_MVP * position;
	v_textureID = textureID;
	v_TexCoord = texCoords;
}
