#pragma once

#include "Renderer.h"
#include "Texture.h"
#include "Window.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct MenuRenderData
{
	std::unique_ptr<VertexArray> VA;
	std::unique_ptr<VertexBuffer> VB;
	std::unique_ptr<IndexBuffer> IB;
	std::unique_ptr<Shader> shader;
	std::unique_ptr<Texture> Title;
	std::unique_ptr<Texture> Text;
	std::unique_ptr<Texture> LevelSelect;
	std::unique_ptr<Texture> SettingsIcon;
	std::unique_ptr<Texture> Button;
	std::unique_ptr<Texture> Exit;
	std::unique_ptr<Texture> Death;
	std::unique_ptr<Texture> Retry;
	std::unique_ptr<Texture> Home;
	std::unique_ptr<Texture> Complete;
};


class Menu
{
private:
	unsigned char CurrentScreen = 0;

	float TextFader = 0.0f;
	float FadeInc = 0.01f;

	glm::mat4 proj = glm::ortho(0.0f, Window::Width, 0.0f, Window::Height, -1.0f, 1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

	MenuRenderData MenuData;

public:
	Menu();
	~Menu();

	void SetCurrentMenu(unsigned char screen);
	void Bind();
	void Draw();
};