#version 330 core

uniform sampler2DArray u_Textures;

in float v_textureID;
in vec2 v_TexCoord;
out vec4 F_Color;

void main()
{
	int index = int(v_textureID);
	if (index == 2147483647) // for unclear reason when v_textureID passed as -1 it is interpretted as INT_MAX
		F_Color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	else if (index % 2 == 0)
		F_Color = texture(u_Textures, vec3(v_TexCoord.x, v_TexCoord.y / 2, index / 2));
	else
		F_Color = texture(u_Textures, vec3(v_TexCoord.x, v_TexCoord.y / 2 + 0.5f, index / 2));
}