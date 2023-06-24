#shader vertex
#version 460 core

layout(location = 0) in float TileID;

out vec2 v_TexCoord;

uniform int ChunkSize;
uniform float ChunkWidth;
uniform int ChunkTilesX;
uniform float TileSize;
uniform float TileInfo[8 * 8 * 4 * 2];
uniform mat4 u_MVP;

void main()
{
	int TileTC = (gl_VertexID % 4) * 2 + int(TileID) * 8;
	v_TexCoord = vec2(TileInfo[TileTC], TileInfo[TileTC + 1]);

	int ChunkID = gl_VertexID / (ChunkSize * 4);

	int TilePos = int(gl_VertexID / 4) % ChunkSize;

	float xPos = (TilePos % ChunkTilesX) * TileSize + ChunkID * ChunkWidth;
	float yPos = int(TilePos / ChunkTilesX) * TileSize;

	vec4 Positions[4] = {
		vec4(xPos,            yPos,            0.0f, 1.0f),
		vec4(xPos + TileSize, yPos,            0.0f, 1.0f),
		vec4(xPos + TileSize, yPos + TileSize, 0.0f, 1.0f),
		vec4(xPos,            yPos + TileSize, 0.0f, 1.0f)
	};

	gl_Position = u_MVP * Positions[gl_VertexID % 4];
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