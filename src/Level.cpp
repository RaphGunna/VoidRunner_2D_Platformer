#include "Level.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

static const size_t MaxQuadCount = Chunks * ChunkSize;
static const size_t MaxVertexCount = MaxQuadCount * 4;
static const size_t MaxIndexCount = MaxQuadCount * 6;
static const size_t MaxTextures = 32;

enum LevelCompleteStates {
    LevelPlaying = 4,
    LevelDeath = 5,
    LevelComplete = 6 
};

Level::Level(unsigned char ID)
{
    LevelID = ID;


    LevelData.VA = std::make_unique<VertexArray>();
    LevelData.VA->Bind();

    LevelData.VB = std::make_unique<VertexBuffer>(nullptr, MaxVertexCount * sizeof(unsigned char));
    LevelData.VB->Bind();

    VertexBufferLayout layout;
    layout.Push<unsigned char>(1);

    LevelData.VA->AddBuffer(*LevelData.VB, layout);

    unsigned int* indices = new unsigned int[MaxIndexCount];
    unsigned int offset = 0;
    for (size_t i = 0; i < MaxIndexCount; i += 6)
    {
        indices[i + 0] = 0 + offset;
        indices[i + 1] = 1 + offset;
        indices[i + 2] = 2 + offset;

        indices[i + 3] = 2 + offset;
        indices[i + 4] = 3 + offset;
        indices[i + 5] = 0 + offset;

        offset += 4;
    }

    LevelData.IB = std::make_unique<IndexBuffer>(indices, MaxIndexCount);
    LevelData.IB->Bind();

    delete[] indices;
    indices = nullptr;

    LevelData.shader = std::make_unique<Shader>("res/shaders/Level.shader");
    LevelData.shader->Bind();
    LevelData.shader->SetUniformMat4f("u_MVP", proj * view * model);

    auto Texloc = glGetUniformLocation(LevelData.shader->GetRendererID(), "u_Textures");
    int TexIDs[1];
    for (int i = 0; i < 1; i++)
        TexIDs[i] = 6;
    glUniform1iv(Texloc, 1, TexIDs);

    LevelData.TileAtlas = std::make_unique<Texture>("res/textures/TileSet.png");
    glBindTextureUnit(6, LevelData.TileAtlas->GetRendererID());

    float TileInfo[8 * 8 * 4 * 2] = {
        0.0f / 24.0f, 22.0f / 24.0f,
        2.0f / 24.0f, 22.0f / 24.0f,
        2.0f / 24.0f, 24.0f / 24.0f,
        0.0f / 24.0f, 24.0f / 24.0f,
        3.0f / 24.0f, 22.0f / 24.0f,
        5.0f / 24.0f, 22.0f / 24.0f,
        5.0f / 24.0f, 24.0f / 24.0f,
        3.0f / 24.0f, 24.0f / 24.0f,
        6.0f / 24.0f, 22.0f / 24.0f,
        8.0f / 24.0f, 22.0f / 24.0f,
        8.0f / 24.0f, 24.0f / 24.0f,
        6.0f / 24.0f, 24.0f / 24.0f,
        9.0f / 24.0f, 22.0f / 24.0f,
        11.0f / 24.0f, 22.0f / 24.0f,
        11.0f / 24.0f, 24.0f / 24.0f,
        9.0f / 24.0f, 24.0f / 24.0f,
        12.0f / 24.0f, 22.0f / 24.0f,
        14.0f / 24.0f, 22.0f / 24.0f,
        14.0f / 24.0f, 24.0f / 24.0f,
        12.0f / 24.0f, 24.0f / 24.0f,
        15.0f / 24.0f, 22.0f / 24.0f,
        17.0f / 24.0f, 22.0f / 24.0f,
        17.0f / 24.0f, 24.0f / 24.0f,
        15.0f / 24.0f, 24.0f / 24.0f,
        18.0f / 24.0f, 22.0f / 24.0f,
        20.0f / 24.0f, 22.0f / 24.0f,
        20.0f / 24.0f, 24.0f / 24.0f,
        18.0f / 24.0f, 24.0f / 24.0f,
        21.0f / 24.0f, 22.0f / 24.0f,
        23.0f / 24.0f, 22.0f / 24.0f,
        23.0f / 24.0f, 24.0f / 24.0f,
        21.0f / 24.0f, 24.0f / 24.0f,
        0.0f / 24.0f, 19.0f / 24.0f,
        2.0f / 24.0f, 19.0f / 24.0f,
        2.0f / 24.0f, 21.0f / 24.0f,
        0.0f / 24.0f, 21.0f / 24.0f,
        3.0f / 24.0f, 19.0f / 24.0f,
        5.0f / 24.0f, 19.0f / 24.0f,
        5.0f / 24.0f, 21.0f / 24.0f,
        3.0f / 24.0f, 21.0f / 24.0f,
        6.0f / 24.0f, 19.0f / 24.0f,
        8.0f / 24.0f, 19.0f / 24.0f,
        8.0f / 24.0f, 21.0f / 24.0f,
        6.0f / 24.0f, 21.0f / 24.0f,
        9.0f / 24.0f, 19.0f / 24.0f,
        11.0f / 24.0f, 19.0f / 24.0f,
        11.0f / 24.0f, 21.0f / 24.0f,
        9.0f / 24.0f, 21.0f / 24.0f,
        12.0f / 24.0f, 19.0f / 24.0f,
        14.0f / 24.0f, 19.0f / 24.0f,
        14.0f / 24.0f, 21.0f / 24.0f,
        12.0f / 24.0f, 21.0f / 24.0f,
        15.0f / 24.0f, 19.0f / 24.0f,
        17.0f / 24.0f, 19.0f / 24.0f,
        17.0f / 24.0f, 21.0f / 24.0f,
        15.0f / 24.0f, 21.0f / 24.0f,
        18.0f / 24.0f, 19.0f / 24.0f,
        20.0f / 24.0f, 19.0f / 24.0f,
        20.0f / 24.0f, 21.0f / 24.0f,
        18.0f / 24.0f, 21.0f / 24.0f,
        21.0f / 24.0f, 19.0f / 24.0f,
        23.0f / 24.0f, 19.0f / 24.0f,
        23.0f / 24.0f, 21.0f / 24.0f,
        21.0f / 24.0f, 21.0f / 24.0f,
        0.0f / 24.0f, 16.0f / 24.0f,
        2.0f / 24.0f, 16.0f / 24.0f,
        2.0f / 24.0f, 18.0f / 24.0f,
        0.0f / 24.0f, 18.0f / 24.0f,
        3.0f / 24.0f, 16.0f / 24.0f,
        5.0f / 24.0f, 16.0f / 24.0f,
        5.0f / 24.0f, 18.0f / 24.0f,
        3.0f / 24.0f, 18.0f / 24.0f,
        6.0f / 24.0f, 16.0f / 24.0f,
        8.0f / 24.0f, 16.0f / 24.0f,
        8.0f / 24.0f, 18.0f / 24.0f,
        6.0f / 24.0f, 18.0f / 24.0f,
        9.0f / 24.0f, 16.0f / 24.0f,
        11.0f / 24.0f, 16.0f / 24.0f,
        11.0f / 24.0f, 18.0f / 24.0f,
        9.0f / 24.0f, 18.0f / 24.0f,
        12.0f / 24.0f, 16.0f / 24.0f,
        14.0f / 24.0f, 16.0f / 24.0f,
        14.0f / 24.0f, 18.0f / 24.0f,
        12.0f / 24.0f, 18.0f / 24.0f,
        15.0f / 24.0f, 16.0f / 24.0f,
        17.0f / 24.0f, 16.0f / 24.0f,
        17.0f / 24.0f, 18.0f / 24.0f,
        15.0f / 24.0f, 18.0f / 24.0f,
        18.0f / 24.0f, 16.0f / 24.0f,
        20.0f / 24.0f, 16.0f / 24.0f,
        20.0f / 24.0f, 18.0f / 24.0f,
        18.0f / 24.0f, 18.0f / 24.0f,
        21.0f / 24.0f, 16.0f / 24.0f,
        23.0f / 24.0f, 16.0f / 24.0f,
        23.0f / 24.0f, 18.0f / 24.0f,
        21.0f / 24.0f, 18.0f / 24.0f,
        0.0f / 24.0f, 13.0f / 24.0f,
        2.0f / 24.0f, 13.0f / 24.0f,
        2.0f / 24.0f, 15.0f / 24.0f,
        0.0f / 24.0f, 15.0f / 24.0f,
        3.0f / 24.0f, 13.0f / 24.0f,
        5.0f / 24.0f, 13.0f / 24.0f,
        5.0f / 24.0f, 15.0f / 24.0f,
        3.0f / 24.0f, 15.0f / 24.0f,
        6.0f / 24.0f, 13.0f / 24.0f,
        8.0f / 24.0f, 13.0f / 24.0f,
        8.0f / 24.0f, 15.0f / 24.0f,
        6.0f / 24.0f, 15.0f / 24.0f,
        9.0f / 24.0f, 13.0f / 24.0f,
        11.0f / 24.0f, 13.0f / 24.0f,
        11.0f / 24.0f, 15.0f / 24.0f,
        9.0f / 24.0f, 15.0f / 24.0f,
        12.0f / 24.0f, 13.0f / 24.0f,
        14.0f / 24.0f, 13.0f / 24.0f,
        14.0f / 24.0f, 15.0f / 24.0f,
        12.0f / 24.0f, 15.0f / 24.0f,
        15.0f / 24.0f, 13.0f / 24.0f,
        17.0f / 24.0f, 13.0f / 24.0f,
        17.0f / 24.0f, 15.0f / 24.0f,
        15.0f / 24.0f, 15.0f / 24.0f,
        18.0f / 24.0f, 13.0f / 24.0f,
        20.0f / 24.0f, 13.0f / 24.0f,
        20.0f / 24.0f, 15.0f / 24.0f,
        18.0f / 24.0f, 15.0f / 24.0f,
        21.0f / 24.0f, 13.0f / 24.0f,
        23.0f / 24.0f, 13.0f / 24.0f,
        23.0f / 24.0f, 15.0f / 24.0f,
        21.0f / 24.0f, 15.0f / 24.0f,
        0.0f / 24.0f, 10.0f / 24.0f,
        2.0f / 24.0f, 10.0f / 24.0f,
        2.0f / 24.0f, 12.0f / 24.0f,
        0.0f / 24.0f, 12.0f / 24.0f,
        3.0f / 24.0f, 10.0f / 24.0f,
        5.0f / 24.0f, 10.0f / 24.0f,
        5.0f / 24.0f, 12.0f / 24.0f,
        3.0f / 24.0f, 12.0f / 24.0f,
        6.0f / 24.0f, 10.0f / 24.0f,
        8.0f / 24.0f, 10.0f / 24.0f,
        8.0f / 24.0f, 12.0f / 24.0f,
        6.0f / 24.0f, 12.0f / 24.0f,
        9.0f / 24.0f, 10.0f / 24.0f,
        11.0f / 24.0f, 10.0f / 24.0f,
        11.0f / 24.0f, 12.0f / 24.0f,
        9.0f / 24.0f, 12.0f / 24.0f,
        12.0f / 24.0f, 10.0f / 24.0f,
        14.0f / 24.0f, 10.0f / 24.0f,
        14.0f / 24.0f, 12.0f / 24.0f,
        12.0f / 24.0f, 12.0f / 24.0f,
        15.0f / 24.0f, 10.0f / 24.0f,
        17.0f / 24.0f, 10.0f / 24.0f,
        17.0f / 24.0f, 12.0f / 24.0f,
        15.0f / 24.0f, 12.0f / 24.0f,
        18.0f / 24.0f, 10.0f / 24.0f,
        20.0f / 24.0f, 10.0f / 24.0f,
        20.0f / 24.0f, 12.0f / 24.0f,
        18.0f / 24.0f, 12.0f / 24.0f,
        21.0f / 24.0f, 10.0f / 24.0f,
        23.0f / 24.0f, 10.0f / 24.0f,
        23.0f / 24.0f, 12.0f / 24.0f,
        21.0f / 24.0f, 12.0f / 24.0f,
        0.0f / 24.0f, 7.0f / 24.0f,
        2.0f / 24.0f, 7.0f / 24.0f,
        2.0f / 24.0f, 9.0f / 24.0f,
        0.0f / 24.0f, 9.0f / 24.0f,
        3.0f / 24.0f, 7.0f / 24.0f,
        5.0f / 24.0f, 7.0f / 24.0f,
        5.0f / 24.0f, 9.0f / 24.0f,
        3.0f / 24.0f, 9.0f / 24.0f,
        6.0f / 24.0f, 7.0f / 24.0f,
        8.0f / 24.0f, 7.0f / 24.0f,
        8.0f / 24.0f, 9.0f / 24.0f,
        6.0f / 24.0f, 9.0f / 24.0f,
        9.0f / 24.0f, 7.0f / 24.0f,
        11.0f / 24.0f, 7.0f / 24.0f,
        11.0f / 24.0f, 9.0f / 24.0f,
        9.0f / 24.0f, 9.0f / 24.0f,
        12.0f / 24.0f, 7.0f / 24.0f,
        14.0f / 24.0f, 7.0f / 24.0f,
        14.0f / 24.0f, 9.0f / 24.0f,
        12.0f / 24.0f, 9.0f / 24.0f,
        15.0f / 24.0f, 7.0f / 24.0f,
        17.0f / 24.0f, 7.0f / 24.0f,
        17.0f / 24.0f, 9.0f / 24.0f,
        15.0f / 24.0f, 9.0f / 24.0f,
        18.0f / 24.0f, 7.0f / 24.0f,
        20.0f / 24.0f, 7.0f / 24.0f,
        20.0f / 24.0f, 9.0f / 24.0f,
        18.0f / 24.0f, 9.0f / 24.0f,
        21.0f / 24.0f, 7.0f / 24.0f,
        23.0f / 24.0f, 7.0f / 24.0f,
        23.0f / 24.0f, 9.0f / 24.0f,
        21.0f / 24.0f, 9.0f / 24.0f,
        0.0f / 24.0f, 4.0f / 24.0f,
        2.0f / 24.0f, 4.0f / 24.0f,
        2.0f / 24.0f, 6.0f / 24.0f,
        0.0f / 24.0f, 6.0f / 24.0f,
        3.0f / 24.0f, 4.0f / 24.0f,
        5.0f / 24.0f, 4.0f / 24.0f,
        5.0f / 24.0f, 6.0f / 24.0f,
        3.0f / 24.0f, 6.0f / 24.0f,
        6.0f / 24.0f, 4.0f / 24.0f,
        8.0f / 24.0f, 4.0f / 24.0f,
        8.0f / 24.0f, 6.0f / 24.0f,
        6.0f / 24.0f, 6.0f / 24.0f,
        9.0f / 24.0f, 4.0f / 24.0f,
        11.0f / 24.0f, 4.0f / 24.0f,
        11.0f / 24.0f, 6.0f / 24.0f,
        9.0f / 24.0f, 6.0f / 24.0f,
        18.0f / 24.0f, 1.0f / 24.0f,
        20.0f / 24.0f, 1.0f / 24.0f,
        20.0f / 24.0f, 3.0f / 24.0f,
        18.0f / 24.0f, 3.0f / 24.0f,
        21.0f / 24.0f, 1.0f / 24.0f,
        23.0f / 24.0f, 1.0f / 24.0f,
        23.0f / 24.0f, 3.0f / 24.0f,
        21.0f / 24.0f, 3.0f / 24.0f,
    };

    LevelData.shader->SetUniform1i("ChunkSize", ChunkSize);

    LevelData.shader->SetUniform1f("ChunkWidth", (float)ChunkWidth);

    LevelData.shader->SetUniform1i("ChunkTilesX", (int)ChunkTilesX);

    LevelData.shader->SetUniform1f("TileSize", (float)TileSize);

    auto TCloc = glGetUniformLocation(LevelData.shader->GetRendererID(), "TileInfo");
    glUniform1fv(TCloc, 8 * 8 * 4 * 2, TileInfo);
    
    switch (LevelID) {
    case 0:
        LoadLevel("res/levels/Level1.txt");
        WallSpeed = 3.0f;
        SetSpawnPoint(288.0f, 384.0f);
        break;
    case 1:
        LoadLevel("res/levels/Level2.txt");
        WallSpeed = 6.0f;
        SetSpawnPoint(544.0f, 320.0f);
        break;
    case 2:
        LoadLevel("res/levels/Level3.txt");
        WallSpeed = 6.0f;
        SetSpawnPoint(896.0f, 320.0f);
        break;
    case 3:
        LoadLevel("res/levels/Test.txt");
        WallSpeed = 3.0f;
        SetSpawnPoint(896.0f, 320.0f);
        break;
    }
}

Level::~Level()
{
}

void Level::SetSpawnPoint(float x, float y)
{
    player.SetPosX(x);
    player.SetPosY(y);
}

void Level::Bind()
{
    LevelData.VA->Bind();
    LevelData.VB->Bind();
    LevelData.IB->Bind();
    LevelData.shader->Bind();
}

void Level::EntBind()
{
    EntData.VA->Bind();
    EntData.VB->Bind();
    EntData.IB->Bind();
    EntData.shader->Bind();
}

void Level::LoadLevel(std::string LevelPath)
{
    Bind();
    unsigned char* VertArray = new unsigned char[MaxVertexCount];
    std::vector<float> EntPosVector;

    std::ifstream LevelFile;
    LevelFile.open(LevelPath);

    unsigned int i = 0;
    for (int chunk = 0; chunk < Chunks; chunk++) {
        unsigned int y = 0;
        unsigned int x = 0;
        std::string ChunkData = "";
        std::getline(LevelFile, ChunkData);
        for (int c = 0; c < ChunkTilesX * ChunkTilesY * 2; c += 2) {
            std::string buffer = "";
            buffer += ChunkData[c];
            buffer += ChunkData[c + 1];
            unsigned char Tile = std::stoi(buffer, 0, 16);
            VertArray[i] = Tile; VertArray[i + 1] = Tile; VertArray[i + 2] = Tile; VertArray[i + 3] = Tile;
            i += 4;
            TileCollider[chunk * ChunkSize + x + y * ChunkTilesX] = Tile;
            x++;
            if (x == 60) {
                y++;
                x = 0;
            }
        }

        std::string EntityData("");
        std::getline(LevelFile, EntityData);
        std::string::difference_type count = std::count(EntityData.begin(), EntityData.end(), ',');
        if (count > 0) {
            std::stringstream EntityString(EntityData);
            std::string EntityTileX = "";
            std::string EntityTileY = "";
            std::string EntityID = "";

            unsigned int EntChunk = 0;
            unsigned int EntTileX = 0;
            unsigned int EntTileY = 0;

            for (int i = 0; i < count / 3; i++) {
                std::getline(EntityString, EntityTileX, ',');
                std::getline(EntityString, EntityTileY, ',');
                std::getline(EntityString, EntityID, ',');

                EntChunk = int(std::stoi(EntityTileX) / 60);
                EntTileX = std::stoi(EntityTileX) % 60;
                EntTileY = std::stoi(EntityTileY);

                unsigned int GlobalTile = EntChunk * ChunkSize + EntTileX + EntTileY * ChunkTilesX;

                float PosX = EntChunk * ChunkWidth + EntTileX * TileSize;
                float PosY = EntTileY * TileSize;

                if (std::stoi(EntityID) <= 4) 
                    Entities[GlobalTile] = new Spike(PosX, PosY, std::stoi(EntityID));
                else 
                    Entities[GlobalTile] = new VoidSlow(PosX, PosY);

                for (int i = 0; i < 4; i++) {
                    int PosAdjusterX = TileSize / 2;
                    int PosAdjusterY = TileSize / 2;

                    if (i < 2)
                        PosAdjusterY *= -1;
                    if (i == 0 || i == 3)
                        PosAdjusterX *= -1;

                    EntPosVector.push_back(Entities[GlobalTile]->GetPosX() + PosAdjusterX);
                    EntPosVector.push_back(Entities[GlobalTile]->GetPosY() + PosAdjusterY);
                    EntPosVector.push_back(Entities[GlobalTile]->GetEntID());
                }
                
            }
        }
    }

    glBufferSubData(GL_ARRAY_BUFFER, 0, MaxVertexCount * sizeof(unsigned char), VertArray);

    delete[] VertArray;
    VertArray = nullptr;

    EntData.VA = std::make_unique<VertexArray>();
    EntData.VA->Bind();

    EntData.VB = std::make_unique<VertexBuffer>(EntPosVector.data(), EntPosVector.size() * sizeof(float));
    EntData.VB->Bind();

    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(1);

    EntData.VA->AddBuffer(*EntData.VB, layout);

    unsigned int* indices = new unsigned int[EntPosVector.size() / 12 * 6];
    unsigned int offset = 0;
    for (size_t i = 0; i < EntPosVector.size() / 12 * 6; i += 6)
    {
        indices[i + 0] = 0 + offset;
        indices[i + 1] = 1 + offset;
        indices[i + 2] = 2 + offset;

        indices[i + 3] = 2 + offset;
        indices[i + 4] = 3 + offset;
        indices[i + 5] = 0 + offset;

        offset += 4;
    }

    EntData.IB = std::make_unique<IndexBuffer>(indices, EntPosVector.size() / 12 * 6);
    EntData.IB->Bind();

    delete[] indices;
    indices = nullptr;

    EntData.shader = std::make_unique<Shader>("res/shaders/Interactables.shader");
    EntData.shader->Bind();
    EntData.shader->SetUniformMat4f("u_MVP", proj * view * model);

    float EntTCs[8 * 4 * 2] = {
        0.0f / 20.0f, 6.0f / 10.0f,
        4.0f / 20.0f, 6.0f / 10.0f,
        4.0f / 20.0f, 10.0f / 10.0f,
        0.0f / 20.0f, 10.0f / 10.0f,
        5.0f / 20.0f, 6.0f / 10.0f,
        9.0f / 20.0f, 6.0f / 10.0f,
        9.0f / 20.0f, 10.0f / 10.0f,
        5.0f / 20.0f, 10.0f / 10.0f,
        10.0f / 20.0f, 6.0f / 10.0f,
        14.0f / 20.0f, 6.0f / 10.0f,
        14.0f / 20.0f, 10.0f / 10.0f,
        10.0f / 20.0f, 10.0f / 10.0f,
        15.0f / 20.0f, 6.0f / 10.0f,
        19.0f / 20.0f, 6.0f / 10.0f,
        19.0f / 20.0f, 10.0f / 10.0f,
        15.0f / 20.0f, 10.0f / 10.0f,
        0.0f / 20.0f, 1.0f / 10.0f,
        4.0f / 20.0f, 1.0f / 10.0f,
        4.0f / 20.0f, 5.0f / 10.0f,
        0.0f / 20.0f, 5.0f / 10.0f,
        5.0f / 20.0f, 1.0f / 10.0f,
        9.0f / 20.0f, 1.0f / 10.0f,
        9.0f / 20.0f, 5.0f / 10.0f,
        5.0f / 20.0f, 5.0f / 10.0f,
        10.0f / 20.0f, 1.0f / 10.0f,
        14.0f / 20.0f, 1.0f / 10.0f,
        14.0f / 20.0f, 5.0f / 10.0f,
        10.0f / 20.0f, 5.0f / 10.0f,
        15.0f / 20.0f, 1.0f / 10.0f,
        19.0f / 20.0f, 1.0f / 10.0f,
        19.0f / 20.0f, 5.0f / 10.0f,
        15.0f / 20.0f, 5.0f / 10.0f
    };

    auto TCloc = glGetUniformLocation(EntData.shader->GetRendererID(), "EntTCs");
    glUniform1fv(TCloc, 8 * 4 * 2, EntTCs);

    auto Texloc = glGetUniformLocation(EntData.shader->GetRendererID(), "u_Textures");
    int TexIDs[1] = { 10 };
    glUniform1iv(Texloc, 1, TexIDs);

    EntData.TileAtlas = std::make_unique<Texture>("res/textures/Interactables.png");
    glBindTextureUnit(10, EntData.TileAtlas->GetRendererID());
}

void Level::UpdateCamera()
{
    Bind();

    CameraPosX = player.GetPosX();
    CameraPosY = player.GetPosY();

    float OffsetX = CameraPosX - Window::Width / 2;
    float OffsetY = CameraPosY - Window::Height / 2;

    float PlayerMoveX = 0.0f;
    float PlayerMoveY = 0.0f;

    if (OffsetX < 0) {
        PlayerMoveX = OffsetX;
        OffsetX = 0;
    }
    else if (OffsetX > LevelWidth - Window::Width) {
        PlayerMoveX = OffsetX - (LevelWidth - Window::Width);
        OffsetX = LevelWidth - Window::Width;
    }
    if (OffsetY < 0) {
        PlayerMoveY = OffsetY;
        OffsetY = 0;
    }
    else if (OffsetY > LevelHeight - Window::Height) {
        PlayerMoveY = OffsetY - (LevelHeight - Window::Height);
        OffsetY = LevelHeight - Window::Height;
    }

    player.SetView(PlayerMoveX, PlayerMoveY, 0);

    view = glm::translate(glm::mat4(1.0f), glm::vec3(-OffsetX, -OffsetY, 0.0f));

    LevelData.shader->SetUniformMat4f("u_MVP", proj * view * model);
    EntBind();
    EntData.shader->SetUniformMat4f("u_MVP", proj * view * model);
}

void Level::UpdateVoid(unsigned int FrameSteps)
{
    if (Void.GetWallPosX() < CameraPosX + Window::Width/2 + 150) {
        Void.SetWallPosX(Void.GetWallPosX() + WallSpeed);
        Void.SetModel();
        Void.SetFrameCount(Void.GetFrameCount() + FrameSteps);
        if (Void.GetFrameCount() > 10) {
            Void.SetFrontState(Void.GetFrontState() + 1);
            if (Void.GetFrontState() >= 4)
                Void.SetFrontState(0);
            Void.SetFrameCount(Void.GetFrameCount() - 10);
        }
        float WallMoveX = 0;
        float WallMoveY = 0;
        if (player.GetPosX() > Window::Width / 2 && player.GetPosX() < LevelWidth - Window::Width / 2)
            WallMoveX = player.GetPosX() - Window::Width / 2;
        else if (player.GetPosX() > LevelWidth - Window::Width / 2)
            WallMoveX = LevelWidth - Window::Width;
        if (player.GetPosY() > Window::Height / 2 && player.GetPosY() < LevelHeight - Window::Height / 2)
            WallMoveY = player.GetPosY() - Window::Height / 2;
        else if (player.GetPosY() > LevelHeight - Window::Height / 2)
            WallMoveY = LevelHeight - Window::Height;

        Void.SetView(WallMoveX, WallMoveY);
    }
}

void Level::UpdateLevelState()
{
    if (player.GetPosX() + TileSize < Void.GetWallPosX() || player.GetDeath())
        LevelState = LevelDeath;
    else if (player.GetPosX() > LevelWidth - 200) {
        LevelState = LevelComplete;

        std::string SaveData;

        std::ifstream FileRead;
        FileRead.open("res/SaveData/SaveData.txt", std::ios::in);
        std::getline(FileRead, SaveData);
        FileRead.close();

        SaveData[LevelID] = 'Y';

        std::ofstream FileWrite;
        FileWrite.open("res/SaveData/SaveData.txt", std::ios::trunc);
        FileWrite << SaveData;
        FileWrite.close();
    }
}

void Level::Draw()
{
    Bind();
    Renderer::Draw(LevelData.VA, LevelData.IB, LevelData.shader);
}

void Level::EntDraw()
{
    EntBind();
    Renderer::Draw(EntData.VA, EntData.IB, EntData.shader);
}

void Level::Update(unsigned int FrameSteps)
{
    if (LevelState == LevelPlaying) {
        player.UpdatePos(TileCollider, SlopeHeights, Entities, FrameSteps);
        UpdateCamera();
        UpdateVoid(FrameSteps);
        UpdateLevelState();
    }
    else if (LevelState == LevelDeath) {
        UpdateVoid(FrameSteps);
    }
}

void Level::Render()
{
    Draw();
    EntDraw();
    player.Draw();
    Void.Draw();
}