#pragma once

#include "Renderer.h"
#include "Texture.h"
#include "Window.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct WallRenderData
{
	std::unique_ptr<VertexArray> VA;
	std::unique_ptr<VertexBuffer> VB;
	std::unique_ptr<IndexBuffer> IB;
	std::unique_ptr<Shader> shader;
	std::unique_ptr<Texture> WallBase;
	std::unique_ptr<Texture> WallFront;
};

class Wall
{
private:
	float WallPosX = 0.0f;
	unsigned char FrameCount = 0;
	unsigned char FrontState = 0;

	glm::mat4 proj = glm::ortho(0.0f, Window::Width, 0.0f, Window::Height, -1.0f, 1.0f);
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

	WallRenderData WallData;

public:
	Wall();
	~Wall();

	void Bind();
	void Draw();

	float GetWallPosX() { return WallPosX; }
	void SetWallPosX(float PosX) { WallPosX = PosX; }
	unsigned char GetFrontState() { return FrontState; }
	void SetFrontState(char State) { FrontState = State; }
	unsigned char GetFrameCount() { return FrameCount; }
	void SetFrameCount(char Count) { FrameCount = Count; }

	void SetModel() { model = glm::translate(glm::mat4(1.0f), glm::vec3(WallPosX, 0, 0)); }
	void SetView(float TransX, float TransY) { view = glm::translate(glm::mat4(1.0f), glm::vec3(-TransX, -TransY, 0)); }
};