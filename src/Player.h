#pragma once

#include "Entity.h"
#include "LevelDimensionInfo.h"

#include "Renderer.h"
#include "Texture.h"
#include "Window.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <unordered_map>

struct PlayerRenderData
{
	std::unique_ptr<VertexArray> VA;
	std::unique_ptr<VertexBuffer> VB;
	std::unique_ptr<IndexBuffer> IB;
	std::unique_ptr<Shader> shader;
	std::unique_ptr<Texture> SpriteSheet;
};

struct Input
{
	bool left = false;
	bool right = false;
	bool jump = false;
	bool PlayerOnGround = false;
	bool WallSliding = false;
	bool debug = false;
};

class Player
{
public:
	const static float WIDTH;
	const static float HEIGHT;
	const static float SizeFixX;
	const static float SizeFixY;

	static bool Paused;

	static Input input;

	static void ResetInput() { 
		input.left = false;
		input.right = false;
	}

private:
	float PlayerPosX = 0.0f;
	float PlayerPosY = 0.0f;
	float PlayerVelX = 0.0f;
	float PlayerVelY = 0.0f;

	unsigned int FrameCounter = 0;
	float AnimFrame = 0.0f;

	unsigned char PreviousSpriteState;
	unsigned char SpriteState;

	bool ApproachingSlope = false;
	bool LeavingSlope = false;
	bool OnSlope = false;

	unsigned int CurrentTiles[12];
	float CurrentRotation = 0.0f;

	bool Death = false;
	bool Slowed = false;

	float DeathFade = 1.0f;
	unsigned int JumpTracker = 0;

	glm::mat4 proj = glm::ortho(0.0f, Window::Width, 0.0f, Window::Height, -1.0f, 1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(Window::Width/2, Window::Height/2, 0));

	PlayerRenderData PlayerData;

public:
	Player();
	~Player();
	
	bool TileCollision(char bound, bool TouchingSlope, unsigned char TileCollider[Chunks * ChunkSize]);
	bool SlopeCollision(char bound, unsigned char TileCollider[Chunks * ChunkSize]);

	bool CheckIfOnSlope(float x, float& y, unsigned char TileCollider[Chunks * ChunkSize], unsigned char SlopeHeights[12 * 2]);
	void HandleSlopeCollision(float x, float& y, unsigned char TileCollider[Chunks * ChunkSize], unsigned char SlopeHeights[12 * 2]);

	void UpdateCurrentTiles(char bound, float x, float y);
	void HandleInput(unsigned int FrameSteps);
	void UpdateSprite(unsigned int FrameSteps);
	void UpdatePos(unsigned char TileCollider[Chunks * ChunkSize], unsigned char SlopeHeights[12 * 2], std::unordered_map<unsigned int, Interactable*>& Entities ,unsigned int FrameSteps);

	void HandleEntities(char bound, std::unordered_map<unsigned int, Interactable*>& Entities);

	void Bind();
	void Draw();

	float GetPosX() { return PlayerPosX; }
	void SetPosX(float x) { PlayerPosX = x; }
	float GetPosY() { return PlayerPosY; }
	void SetPosY(float y) { PlayerPosY = y; }
	bool GetDeath() { return Death; }
	void SetView(float x, float y, float z) { view = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z)); }
};