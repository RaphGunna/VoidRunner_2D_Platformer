#shader vertex
#version 460 core

layout(location = 0) in vec4 Position;
layout(location = 1) in float TexID;

out vec2 v_TexCoord;
out float v_TexID;

uniform float Frame;
uniform float FrameInfo[4 * 4 * 2];
uniform mat4 u_MVP;

void main()
{	
	if (gl_VertexID >= 4) {
		int FrameOffset = int(Frame) * 4 * 2;
		v_TexCoord = vec2(FrameInfo[FrameOffset + (gl_VertexID % 4) * 2], FrameInfo[FrameOffset + (gl_VertexID % 4) * 2 + 1]);
	}
	else {
		float TCs[8] = {
			0.0f, 0.0f,
			1920.0f/1980.0f, 0.0f,
			1920.0f / 1980.0f, 1.0f,
			0.0f, 1.0f
		};
		v_TexCoord = vec2(TCs[gl_VertexID * 2], TCs[gl_VertexID * 2 + 1]);
	}

	v_TexID = TexID - 8.0f;
	gl_Position = u_MVP * Position;
};

#shader fragment
#version 460 core

layout(location = 0) out vec4 FragColor;

in vec2 v_TexCoord;
in float v_TexID;

uniform sampler2D u_Textures[2];

void main()
{
	FragColor = texture(u_Textures[int(v_TexID)], v_TexCoord);
};