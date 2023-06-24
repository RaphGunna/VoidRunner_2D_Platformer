#shader vertex
#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) in float TexID;

uniform float CompleteStatus[3];
uniform float LevelIconTCs[6 * 4 * 2];
uniform mat4 u_MVP;

out vec2 TexCoord;
out float tex;

void main()
{
	if (int(TexID) == 2) {
		int Index = (gl_VertexID - 8) * 2 + int(CompleteStatus[int((gl_VertexID - 8) / 4)]);

		TexCoord = vec2(LevelIconTCs[Index], LevelIconTCs[Index + 1]);
	}
	else {
		vec2 TCs[4] = {
			vec2(0.0f, 0.0f),
			vec2(1.0f, 0.0f),
			vec2(1.0f, 1.0f),
			vec2(0.0f, 1.0f),
		};
		TexCoord = TCs[gl_VertexID % 4];
	}

	tex = TexID;

	gl_Position = u_MVP * position;
};

#shader fragment
#version 460 core

layout(location = 0) out vec4 Texture;

uniform sampler2D u_Textures[32];
uniform float Fader;

in vec2 TexCoord;
in float tex;

void main()
{
	vec4 fade = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	if (int(tex) == 1) {
		fade = vec4(1.0f - Fader, 1.0f - Fader, 1.0f - Fader, 1.0f);
	}
	Texture = texture(u_Textures[int(tex)], TexCoord) * fade;
};