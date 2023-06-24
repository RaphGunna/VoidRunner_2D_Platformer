#shader vertex
#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) in float texIndex;

uniform float AnimOffset;
uniform float AnimFrame;
uniform float AnimDirection;
uniform float SpriteTCs[8 * 6 * 4 * 2];
uniform mat4 u_MVP;

out vec2 v_TexCoord;

void main()
{
	int TCbase = int(AnimOffset) + int(AnimFrame) * 8 + gl_VertexID * 2;

	if (gl_VertexID % 2 == 0)
		TCbase = TCbase + int(AnimDirection) * 2;
	else
		TCbase = TCbase - int(AnimDirection) * 2;

	v_TexCoord = vec2(SpriteTCs[TCbase], SpriteTCs[TCbase + 1]);
	gl_Position = u_MVP * position;
};

#shader fragment
#version 460 core

layout(location = 0) out vec4 FragColor;

in vec2 v_TexCoord;

uniform float DeathFade;
uniform sampler2D u_Textures[1];

void main()
{
	FragColor = texture(u_Textures[0], v_TexCoord) * vec4(DeathFade, DeathFade, DeathFade, 1.0f);
};