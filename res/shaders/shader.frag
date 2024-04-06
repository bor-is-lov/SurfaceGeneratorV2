#version 330 core

uniform sampler2DArray u_Textures;

in float v_textureID;
in vec2 v_TexCoord;
out vec4 F_Color;

void main()
{
	int index = int(v_textureID);
	F_Color = texture(u_Textures, vec3(v_TexCoord, index));
}