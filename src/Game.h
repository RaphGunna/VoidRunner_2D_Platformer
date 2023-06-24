#pragma once
#include "Menu.h"
#include "Level.h"
#include "Window.h"

class Game
{
public:
	static bool GameStarted;
	static unsigned char GameState;

	static Menu* menu;
	static Level* level;

	static void TitleCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void LevelSelectCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void SettingsCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void PlayerCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void InGamePausedCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void LevelDeathCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void LevelCompleteCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

public:
	Game();
	~Game();

	void PlayGame();
};