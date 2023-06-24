#include "Menu.h"

#include <fstream>
#include <string>

enum GameStates
{
    Title = 0,
    LevelSelect = 1,
    Settings = 2,
    LevelDeath = 5,
    LevelComplete = 6
};

Menu::Menu()
{
    MenuData.VA = std::make_unique<VertexArray>();
    MenuData.VA->Bind();

    // Title Screen
    float LogoPosX = Window::Width / 2;
    float LogoSizeX = 580.0f * 2 / 2;
    float LogoPosY = Window::Height / 2 * 1.5f;
    float LogoSizeY = 90.0f * 2 / 2;

    float TextPosX = Window::Width / 2;
    float TextSizeX = 590.0f * 1 / 2;
    float TextPosY = Window::Height / 2 - 200;
    float TextSizeY = 45.0f * 1 / 2;

    // Home Screen
    float Level1PosX = Window::Width / 2 - 256;
    float Level2PosX = Window::Width / 2;
    float Level3PosX = Window::Width / 2 + 256;
    float LevelSizeX = 256.0f * 1 / 2;
    float LevelPosY = Window::Height / 2;
    float LevelSizeY = 256.0f * 1 / 2;

    float SettingsPosX = Window::Width - 200;
    float SettingsSizeX = 100.0f * 1 / 2;
    float SettingsPosY = 200;
    float SettingsSizeY = 100.0f * 1 / 2;

    // Settings Screen
    float ButtonPosX = Window::Width / 2;
    float ButtonSizeX = 300.0f * 1 / 2;
    float ButtonPosY = Window::Height / 2;
    float ButtonSizeY = 150.0f * 1 / 2;

    float ExitPosX = Window::Width / 2 + 200;
    float ExitSizeX = 100.0f * 1 / 2;
    float ExitPosY = Window::Height / 2 + 200;
    float ExitSizeY = 100.0f * 1 / 2;

    // LevelDeath Screen
    float DeathPosX = Window::Width / 2;
    float DeathSizeX = 400.0f * 1 / 2;
    float DeathPosY = Window::Height / 2 + 300;
    float DeathSizeY = 70.0f * 1 / 2;

    float RetryPosX = Window::Width / 2;
    float RetrySizeX = 430.0f * 1 / 2;
    float RetryPosY = Window::Height / 2;
    float RetrySizeY = 170.0f * 1 / 2;

    float HomePosX = Window::Width / 2;
    float HomeSizeX = 430.0f * 1 / 2;
    float HomePosY = Window::Height / 2 - 300;
    float HomeSizeY = 170.0f * 1 / 2;

    // Level Complete Screen
    float CompletePosX = Window::Width / 2;
    float CompleteSizeX = 760.0f * 1 / 2;
    float CompletePosY = Window::Height / 2 + 300;
    float CompleteSizeY = 70.0f * 1 / 2;

    float DisplayPosition[] = {
        // Title
        LogoPosX - LogoSizeX, LogoPosY - LogoSizeY, 0,
        LogoPosX + LogoSizeX, LogoPosY - LogoSizeY, 0,
        LogoPosX + LogoSizeX, LogoPosY + LogoSizeY, 0,
        LogoPosX - LogoSizeX, LogoPosY + LogoSizeY, 0,

        TextPosX - TextSizeX, TextPosY - TextSizeY, 1,
        TextPosX + TextSizeX, TextPosY - TextSizeY, 1,
        TextPosX + TextSizeX, TextPosY + TextSizeY, 1,
        TextPosX - TextSizeX, TextPosY + TextSizeY, 1,

        // Home
        Level1PosX - LevelSizeX, LevelPosY - LevelSizeY, 2,
        Level1PosX + LevelSizeX, LevelPosY - LevelSizeY, 2,
        Level1PosX + LevelSizeX, LevelPosY + LevelSizeY, 2,
        Level1PosX - LevelSizeX, LevelPosY + LevelSizeY, 2,

        Level2PosX - LevelSizeX, LevelPosY - LevelSizeY, 2,
        Level2PosX + LevelSizeX, LevelPosY - LevelSizeY, 2,
        Level2PosX + LevelSizeX, LevelPosY + LevelSizeY, 2,
        Level2PosX - LevelSizeX, LevelPosY + LevelSizeY, 2,

        Level3PosX - LevelSizeX, LevelPosY - LevelSizeY, 2,
        Level3PosX + LevelSizeX, LevelPosY - LevelSizeY, 2,
        Level3PosX + LevelSizeX, LevelPosY + LevelSizeY, 2,
        Level3PosX - LevelSizeX, LevelPosY + LevelSizeY, 2,

        SettingsPosX - SettingsSizeX, SettingsPosY - SettingsSizeY, 3,
        SettingsPosX + SettingsSizeX, SettingsPosY - SettingsSizeY, 3,
        SettingsPosX + SettingsSizeX, SettingsPosY + SettingsSizeY, 3,
        SettingsPosX - SettingsSizeX, SettingsPosY + SettingsSizeY, 3,

        // Settings
        ButtonPosX - ButtonSizeX, ButtonPosY - ButtonSizeY, 4,
        ButtonPosX + ButtonSizeX, ButtonPosY - ButtonSizeY, 4,
        ButtonPosX + ButtonSizeX, ButtonPosY + ButtonSizeY, 4,
        ButtonPosX - ButtonSizeX, ButtonPosY + ButtonSizeY, 4,

        ExitPosX - ExitSizeX, ExitPosY - ExitSizeY, 5,
        ExitPosX + ExitSizeX, ExitPosY - ExitSizeY, 5,
        ExitPosX + ExitSizeX, ExitPosY + ExitSizeY, 5,
        ExitPosX - ExitSizeX, ExitPosY + ExitSizeY, 5,

        // Death
        DeathPosX - DeathSizeX, DeathPosY - DeathSizeY, 11,
        DeathPosX + DeathSizeX, DeathPosY - DeathSizeY, 11,
        DeathPosX + DeathSizeX, DeathPosY + DeathSizeY, 11,
        DeathPosX - DeathSizeX, DeathPosY + DeathSizeY, 11,

        RetryPosX - RetrySizeX, RetryPosY - RetrySizeY, 12,
        RetryPosX + RetrySizeX, RetryPosY - RetrySizeY, 12,
        RetryPosX + RetrySizeX, RetryPosY + RetrySizeY, 12,
        RetryPosX - RetrySizeX, RetryPosY + RetrySizeY, 12,

        HomePosX - HomeSizeX, HomePosY - HomeSizeY, 13,
        HomePosX + HomeSizeX, HomePosY - HomeSizeY, 13,
        HomePosX + HomeSizeX, HomePosY + HomeSizeY, 13,
        HomePosX - HomeSizeX, HomePosY + HomeSizeY, 13,

        // Complete
        CompletePosX - CompleteSizeX, CompletePosY - CompleteSizeY, 14,
        CompletePosX + CompleteSizeX, CompletePosY - CompleteSizeY, 14,
        CompletePosX + CompleteSizeX, CompletePosY + CompleteSizeY, 14,
        CompletePosX - CompleteSizeX, CompletePosY + CompleteSizeY, 14
    };

    MenuData.VB = std::make_unique<VertexBuffer>(DisplayPosition, sizeof(DisplayPosition));
    MenuData.VB->Bind();

    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(1);

    MenuData.VA->AddBuffer(*MenuData.VB, layout);
    
    unsigned int indices[12 * 6];
    unsigned int offset = 0;
    for (size_t i = 0; i < 12 * 6; i += 6)
    {
        indices[i + 0] = 0 + offset;
        indices[i + 1] = 1 + offset;
        indices[i + 2] = 2 + offset;

        indices[i + 3] = 2 + offset;
        indices[i + 4] = 3 + offset;
        indices[i + 5] = 0 + offset;

        offset += 4;
    }

    MenuData.IB = std::make_unique<IndexBuffer>(indices, 12 * 6);
    MenuData.IB->Bind();

    MenuData.shader = std::make_unique<Shader>("res/shaders/Menu.shader");
    MenuData.shader->Bind();
    MenuData.shader->SetUniformMat4f("u_MVP", proj * view * model);

    float LevelIconTCs[6 * 4 * 2] = {
        0.0f / 3.0f, 1.0f / 2.0f,
        1.0f / 3.0f, 1.0f / 2.0f,
        1.0f / 3.0f, 2.0f / 2.0f,
        0.0f / 3.0f, 2.0f / 2.0f,
        1.0f / 3.0f, 1.0f / 2.0f,
        2.0f / 3.0f, 1.0f / 2.0f,
        2.0f / 3.0f, 2.0f / 2.0f,
        1.0f / 3.0f, 2.0f / 2.0f,
        2.0f / 3.0f, 1.0f / 2.0f,
        3.0f / 3.0f, 1.0f / 2.0f,
        3.0f / 3.0f, 2.0f / 2.0f,
        2.0f / 3.0f, 2.0f / 2.0f,
        0.0f / 3.0f, 0.0f / 2.0f,
        1.0f / 3.0f, 0.0f / 2.0f,
        1.0f / 3.0f, 1.0f / 2.0f,
        0.0f / 3.0f, 1.0f / 2.0f,
        1.0f / 3.0f, 0.0f / 2.0f,
        2.0f / 3.0f, 0.0f / 2.0f,
        2.0f / 3.0f, 1.0f / 2.0f,
        1.0f / 3.0f, 1.0f / 2.0f,
        2.0f / 3.0f, 0.0f / 2.0f,
        3.0f / 3.0f, 0.0f / 2.0f,
        3.0f / 3.0f, 1.0f / 2.0f,
        2.0f / 3.0f, 1.0f / 2.0f
    };

    auto TCloc = glGetUniformLocation(MenuData.shader->GetRendererID(), "LevelIconTCs");
    glUniform1fv(TCloc, 6 * 4 * 2, LevelIconTCs);

    float CompleteStatus[3] = { 0.0f, 0.0f, 0.0f };

    auto Statusloc = glGetUniformLocation(MenuData.shader->GetRendererID(), "CompleteStatus");
    glUniform1fv(Statusloc, 3, CompleteStatus);

    MenuData.shader->SetUniform1f("Fader", TextFader);

    auto Texloc = glGetUniformLocation(MenuData.shader->GetRendererID(), "u_Textures");
    int TexIDs[32];
    for (int i = 0; i < 32; i++)
        TexIDs[i] = i;
    glUniform1iv(Texloc, 32, TexIDs);

    MenuData.Title = std::make_unique<Texture>("res/textures/VoidRunner.png");
    MenuData.Text = std::make_unique<Texture>("res/textures/TitleText.png");
    MenuData.LevelSelect = std::make_unique<Texture>("res/textures/LevelIcons.png");
    MenuData.SettingsIcon = std::make_unique<Texture>("res/textures/Settings.png");
    MenuData.Button = std::make_unique<Texture>("res/textures/Quit.png");
    MenuData.Exit = std::make_unique<Texture>("res/textures/Exit.png");
    MenuData.Death = std::make_unique<Texture>("res/textures/You-Died.png");
    MenuData.Retry = std::make_unique<Texture>("res/textures/Retry.png");
    MenuData.Home = std::make_unique<Texture>("res/textures/Home.png");
    MenuData.Complete = std::make_unique<Texture>("res/textures/Level-Complete.png");

    glBindTextureUnit(0, MenuData.Title->GetRendererID());
    glBindTextureUnit(1, MenuData.Text->GetRendererID());
    glBindTextureUnit(2, MenuData.LevelSelect->GetRendererID());
    glBindTextureUnit(3, MenuData.SettingsIcon->GetRendererID());
    glBindTextureUnit(4, MenuData.Button->GetRendererID());
    glBindTextureUnit(5, MenuData.Exit->GetRendererID());
    glBindTextureUnit(11, MenuData.Death->GetRendererID());
    glBindTextureUnit(12, MenuData.Retry->GetRendererID());
    glBindTextureUnit(13, MenuData.Home->GetRendererID());
    glBindTextureUnit(14, MenuData.Complete->GetRendererID());
}

Menu::~Menu()
{
}

void Menu::SetCurrentMenu(unsigned char screen)
{
    Bind();
    int size = 12;
    if (screen == LevelSelect) {
        size = 24;

        float CompleteStatus[3] = {};

        std::string SaveData;

        std::ifstream FileRead;
        FileRead.open("res/SaveData/SaveData.txt", std::ios::in);
        std::getline(FileRead, SaveData);
        FileRead.close();

        for (int i = 0; i < 3; i++) {
            if (SaveData[i] == 'Y')
                CompleteStatus[i] = 24.0f;
        }

        auto Statusloc = glGetUniformLocation(MenuData.shader->GetRendererID(), "CompleteStatus");
        glUniform1fv(Statusloc, 3, CompleteStatus);
    }
    else if (screen == LevelDeath)
        size = 18;

    unsigned int MenuIndices[24] = {};

    if (screen == Title) {
        unsigned int ScreenIndices[] = {
            0, 1, 2,
            2, 3, 0,

            4, 5, 6,
            6, 7, 4
        };
        std::copy(ScreenIndices, ScreenIndices + 12, MenuIndices);
    }
    else if (screen == LevelSelect) {
        unsigned int ScreenIndices[] = {
             8,  9, 10,
            10, 11,  8,

            12, 13, 14,
            14, 15, 12,

            16,  17, 18,
            18,  19,  16,

            20, 21, 22,
            22, 23, 20
        };
        std::copy(ScreenIndices, ScreenIndices + 24, MenuIndices);
    }
    else if (screen == Settings) {
        unsigned int ScreenIndices[] = {
            24,  25,  26,
            26,  27,  24,

            28, 29, 30,
            30, 31, 28,
        };
        std::copy(ScreenIndices, ScreenIndices + 12, MenuIndices);
    }
    else if (screen == LevelDeath) {
        unsigned int ScreenIndices[] = {

            32, 33, 34,
            34, 35, 32,

            36,  37,  38,
            38,  39,  36,

            40, 41, 42,
            42, 43, 40
        };
        std::copy(ScreenIndices, ScreenIndices + 18, MenuIndices);
    }
    else if (screen == LevelComplete) {
        unsigned int ScreenIndices[] = {
            44,  45,  46,
            46,  47,  44,

            40, 41, 42,
            42, 43, 40
        };
        std::copy(ScreenIndices, ScreenIndices + 12, MenuIndices);
    }

    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(MenuIndices), MenuIndices);
    MenuData.IB->SetCount(size);
}

void Menu::Bind()
{
    MenuData.VA->Bind();
    MenuData.VB->Bind();
    MenuData.IB->Bind();
    MenuData.shader->Bind();
}

void Menu::Draw()
{
    if (CurrentScreen == Title) {
        if (TextFader > 0.7) FadeInc = -0.01f;
        else if (TextFader < 0) FadeInc = 0.01f;
        TextFader += FadeInc;
        MenuData.shader->SetUniform1f("Fader", TextFader);
    }

    Renderer::Draw(MenuData.VA, MenuData.IB, MenuData.shader);
}