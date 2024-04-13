#version 330 core

uniform sampler2DArray u_Textures;

in float v_textureID;
in vec2 v_TexCoord;
out vec4 F_Color;

void main()
{
	int index = int(v_textureID);
	if (index % 2 == 0)
		F_Color = texture(u_Textures, vec3(v_TexCoord.x, v_TexCoord.y / 2, index / 2));
	else
		F_Color = texture(u_Textures, vec3(v_TexCoord.x, v_TexCoord.y / 2 + 0.5f, index / 2));
}