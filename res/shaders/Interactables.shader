#shader vertex
#version 460 core

layout(location = 0) in vec4 Position;
layout(location = 1) in float EntID;

out vec2 v_TexCoord;

uniform float EntTCs[8 * 4 * 2];
uniform mat4 u_MVP;

void main()
{	
	int TCOffset = (int(EntID) - 1) * 4 * 2;
	v_TexCoord = vec2(EntTCs[TCOffset + (gl_VertexID % 4) * 2], EntTCs[TCOffset + (gl_VertexID % 4) * 2 + 1]);

	gl_Position = u_MVP * Position;
};

#shader fragment
#version 460 core

layout(location = 0) out vec4 FragColor;

in vec2 v_TexCoord;

uniform sampler2D u_Textures[1];

void main()
{
	FragColor = texture(u_Textures[0], v_TexCoord);
};