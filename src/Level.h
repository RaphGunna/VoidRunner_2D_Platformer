#pragma once

#include "Player.h"
#include "Wall.h"
#include "Entity.h"
#include "LevelDimensionInfo.h"

#include "Renderer.h"
#include "Texture.h"
#include "Window.h"

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include <unordered_map>

struct LevelRenderData
{
	std::unique_ptr<VertexArray> VA;
	std::unique_ptr<VertexBuffer> VB;
	std::unique_ptr<IndexBuffer> IB;
	std::unique_ptr<Shader> shader;
	std::unique_ptr<Texture> TileAtlas;
};

class Level
{
private:
	unsigned char LevelID;
	unsigned char LevelState = 4;

	float LevelWidth = Chunks * ChunkWidth;
	float LevelHeight = ChunkTilesY * TileSize;

	LevelRenderData EntData;
	std::unordered_map<unsigned int, Interactable*> Entities;
	
	Player player;
	Wall Void;

	unsigned char* TileCollider = new unsigned char[Chunks * ChunkSize];
	unsigned char* SlopeHeights = new unsigned char[12 * 2]
	{
		1, 16, 17, 32,
		32, 17, 16, 1,
		1, 8, 9, 16, 17, 24, 25, 32,
		32, 25, 24, 17, 16, 9, 8, 1,
	};

	float CameraPosX = 0.0f;
	float CameraPosY = 0.0f;

	float WallSpeed = 0.0f;

	glm::mat4 proj = glm::ortho(0.0f, Window::Width, 0.0f, Window::Height, -1.0f, 1.0f);
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

	LevelRenderData LevelData;

public:
	Level(unsigned char ID);
	~Level();

	void LoadLevel(std::string LevelPath);
	void SetSpawnPoint(float x, float y);

	void UpdateCamera();
	void UpdateVoid(unsigned int FrameSteps);
	void UpdateLevelState();

	void Bind();
	void EntBind();
	void Draw();
	void EntDraw();

	void Update(unsigned int FrameSteps);
	void Render();

	unsigned char GetLevelID() { return LevelID; }
	unsigned char GetLevelState() { return LevelState; }
	unsigned char SetLevelState(unsigned char State) { LevelState = State; }
	std::unordered_map<unsigned int, Interactable*> GetEntities() { return Entities; }
};