#include "Game.h"

#include <chrono>

enum GameStates {
    Title = 0,
    LevelSelect = 1,
    Settings = 2,
    InGame = 3,
    InGamePaused = 4,
    LevelDeath = 5,
    LevelComplete = 6
};

bool Game::GameStarted = false;
unsigned char Game::GameState = Title;
Menu* Game::menu;
Level* Game::level;

Game::Game()
{
    /* Initialize the library */
    glfwInit();

    Window::CreateWindow();

    int version = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    Window::SetKeyCallback(TitleCallback);

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

Game::~Game()
{
    delete menu;
    menu = nullptr;
    Window::DestroyWindow();
}

void Game::TitleCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS && key == GLFW_KEY_SPACE) {
        GameState = LevelSelect;
        Window::SetKeyCallback(LevelSelectCallback);
    }
}

void Game::LevelSelectCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        GameState = Settings;
        Window::SetKeyCallback(SettingsCallback);
    }
    else if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        GameState = InGame;
        level = new Level(0);
        Window::SetKeyCallback(PlayerCallback);
    }
    else if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        GameState = InGame;
        level = new Level(1);
        Window::SetKeyCallback(PlayerCallback);
    }
    else if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
        GameState = InGame;
        level = new Level(2);
        Window::SetKeyCallback(PlayerCallback);
    }
    else if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
        GameState = InGame;
        level = new Level(3);
        Window::SetKeyCallback(PlayerCallback);
    }
}

void Game::SettingsCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        GameState = LevelSelect;
        Window::SetKeyCallback(LevelSelectCallback);
    }
    else if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(Window::window, GLFW_TRUE);
    }
}

void Game::PlayerCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_LEFT) {
        if (action != GLFW_RELEASE)
            Player::input.left = true;
        else
            Player::input.left = false;
    }
    else if (key == GLFW_KEY_RIGHT) {
        if (action != GLFW_RELEASE)
            Player::input.right = true;
        else
            Player::input.right = false;
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        Player::input.jump = true;
    }
    else if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) {
        Player::input.jump = false;
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        GameState = InGamePaused;
        Window::SetKeyCallback(InGamePausedCallback);
    }
}

void Game::InGamePausedCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        GameState = InGame;
        Window::SetKeyCallback(PlayerCallback);
        Player::ResetInput();
    }
    else if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        GameState = LevelSelect;
        Window::SetKeyCallback(LevelSelectCallback);
        delete level;
        level = nullptr;
    }
}

void Game::LevelDeathCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        GameState = LevelSelect;
        Window::SetKeyCallback(LevelSelectCallback);
        delete level;
        level = nullptr;
    }
    else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        GameState = InGame;
        Window::SetKeyCallback(PlayerCallback);
        unsigned char LevelID = level->GetLevelID();
        delete level;
        level = nullptr;
        level = new Level(LevelID);
    }
}

void Game::LevelCompleteCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        GameState = LevelSelect;
        Window::SetKeyCallback(LevelSelectCallback);
        delete level;
        level = nullptr;
    }
}

void Game::PlayGame()
{      
    {
        menu = new Menu();

        double Time = 0.0;
        double DeltaTime = 1.0 / 60.0;

        auto CurrentTime = std::chrono::system_clock::now();
        double Accumulator = 0.0;
        int fps = 0;

        while (!glfwWindowShouldClose(Window::window))
        {
            auto NewTime = std::chrono::system_clock::now();
            std::chrono::duration<double> FrameTime = NewTime - CurrentTime;
            CurrentTime = NewTime;

            Accumulator += FrameTime.count();

            // Main Draw
            if (Accumulator >= DeltaTime) {
                Renderer::Clear();

                unsigned int FrameSteps = int(Accumulator / DeltaTime);
                
                switch (GameState) {
                case Title:
                case LevelSelect:
                case Settings:
                    menu->SetCurrentMenu(GameState);
                    menu->Draw();
                    break;
                case InGame:
                    level->Update(FrameSteps);
                    level->Render();
                    if (level->GetLevelState() == LevelDeath || level->GetLevelState() == LevelComplete) {
                        GameState = level->GetLevelState();
                        if (GameState == LevelDeath)
                            Window::SetKeyCallback(LevelDeathCallback);
                        else
                            Window::SetKeyCallback(LevelCompleteCallback);
                    }
                    break;
                case InGamePaused:
                    level->Render();
                    menu->SetCurrentMenu(Settings);
                    menu->Draw();
                    break;
                case LevelDeath:
                case LevelComplete:
                    level->Update(FrameSteps);
                    level->Render();
                    menu->SetCurrentMenu(GameState);
                    menu->Draw();
                    break;
                }
                
                Time += FrameSteps * DeltaTime;
                Accumulator -= FrameSteps * DeltaTime;

                Window::SwapBuffers();
                Window::PollEvents();
            }
        }
    }
}