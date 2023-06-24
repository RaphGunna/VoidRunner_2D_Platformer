#include "Player.h"

#include <cmath>

const float Player::WIDTH = 64.0f;
const float Player::HEIGHT = 128.0f;

const float Player::SizeFixX = Player::WIDTH - 0.01f;
const float Player::SizeFixY = Player::HEIGHT - 0.01f;

bool Player::Paused = false;

Input Player::input;

unsigned int LowerSlopeBound = 37;
unsigned int UpperSlopeBound = 48;

enum PlayerBounds {
    Left = 1,
    Right = 2,
    Bottom = 3,
    Top = 4
};

enum SpriteStates {
    Idle = 1,
    Jumping = 2,
    WallSlide = 3,
    Walking = 4,
    Running = 5,
    Landing = 6,
    Falling = 7
};

Player::Player()
{
    PlayerData.VA = std::make_unique<VertexArray>();
    PlayerData.VA->Bind();

    float PlayerPosition[] = {
        -128 / 2, -128 / 2, 7.0f,
         128 / 2, -128 / 2, 7.0f,
         128 / 2,  128 / 2, 7.0f,
        -128 / 2,  128 / 2, 7.0f
    };

    PlayerData.VB = std::make_unique<VertexBuffer>(PlayerPosition, sizeof(PlayerPosition));
    PlayerData.VB->Bind();

    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(1);

    PlayerData.VA->AddBuffer(*PlayerData.VB, layout);

    unsigned int indices[6] = {
        0, 1, 2,
        2, 3, 0
    };

    PlayerData.IB = std::make_unique<IndexBuffer>(indices, 6);
    PlayerData.IB->Bind();

    PlayerData.shader = std::make_unique<Shader>("res/shaders/Player.shader");
    PlayerData.shader->Bind();
    PlayerData.shader->SetUniform1f("AnimOffset", 0);
    PlayerData.shader->SetUniform1f("AnimFrame", 0);
    PlayerData.shader->SetUniform1f("AnimDirection", 0);
    PlayerData.shader->SetUniformMat4f("u_MVP", proj * view * model);
    PlayerData.shader->SetUniform1f("DeathFade", DeathFade);

    float IdleTCs[8 * 6 * 4 * 2] = {
        0.0f / 8.0f, 26.0f / 30.0f,
        1.0f / 8.0f, 26.0f / 30.0f,
        1.0f / 8.0f, 30.0f / 30.0f,
        0.0f / 8.0f, 30.0f / 30.0f,
        1.0f / 8.0f, 26.0f / 30.0f,
        2.0f / 8.0f, 26.0f / 30.0f,
        2.0f / 8.0f, 30.0f / 30.0f,
        1.0f / 8.0f, 30.0f / 30.0f,
        2.0f / 8.0f, 26.0f / 30.0f,
        3.0f / 8.0f, 26.0f / 30.0f,
        3.0f / 8.0f, 30.0f / 30.0f,
        2.0f / 8.0f, 30.0f / 30.0f,
        3.0f / 8.0f, 26.0f / 30.0f,
        4.0f / 8.0f, 26.0f / 30.0f,
        4.0f / 8.0f, 30.0f / 30.0f,
        3.0f / 8.0f, 30.0f / 30.0f,
        4.0f / 8.0f, 26.0f / 30.0f,
        5.0f / 8.0f, 26.0f / 30.0f,
        5.0f / 8.0f, 30.0f / 30.0f,
        4.0f / 8.0f, 30.0f / 30.0f,
        5.0f / 8.0f, 26.0f / 30.0f,
        6.0f / 8.0f, 26.0f / 30.0f,
        6.0f / 8.0f, 30.0f / 30.0f,
        5.0f / 8.0f, 30.0f / 30.0f,
        6.0f / 8.0f, 26.0f / 30.0f,
        7.0f / 8.0f, 26.0f / 30.0f,
        7.0f / 8.0f, 30.0f / 30.0f,
        6.0f / 8.0f, 30.0f / 30.0f,
        7.0f / 8.0f, 26.0f / 30.0f,
        8.0f / 8.0f, 26.0f / 30.0f,
        8.0f / 8.0f, 30.0f / 30.0f,
        7.0f / 8.0f, 30.0f / 30.0f,
        0.0f / 8.0f, 21.0f / 30.0f,
        1.0f / 8.0f, 21.0f / 30.0f,
        1.0f / 8.0f, 25.0f / 30.0f,
        0.0f / 8.0f, 25.0f / 30.0f,
        1.0f / 8.0f, 21.0f / 30.0f,
        2.0f / 8.0f, 21.0f / 30.0f,
        2.0f / 8.0f, 25.0f / 30.0f,
        1.0f / 8.0f, 25.0f / 30.0f,
        2.0f / 8.0f, 21.0f / 30.0f,
        3.0f / 8.0f, 21.0f / 30.0f,
        3.0f / 8.0f, 25.0f / 30.0f,
        2.0f / 8.0f, 25.0f / 30.0f,
        3.0f / 8.0f, 21.0f / 30.0f,
        4.0f / 8.0f, 21.0f / 30.0f,
        4.0f / 8.0f, 25.0f / 30.0f,
        3.0f / 8.0f, 25.0f / 30.0f,
        4.0f / 8.0f, 21.0f / 30.0f,
        5.0f / 8.0f, 21.0f / 30.0f,
        5.0f / 8.0f, 25.0f / 30.0f,
        4.0f / 8.0f, 25.0f / 30.0f,
        5.0f / 8.0f, 21.0f / 30.0f,
        6.0f / 8.0f, 21.0f / 30.0f,
        6.0f / 8.0f, 25.0f / 30.0f,
        5.0f / 8.0f, 25.0f / 30.0f,
        6.0f / 8.0f, 21.0f / 30.0f,
        7.0f / 8.0f, 21.0f / 30.0f,
        7.0f / 8.0f, 25.0f / 30.0f,
        6.0f / 8.0f, 25.0f / 30.0f,
        7.0f / 8.0f, 21.0f / 30.0f,
        8.0f / 8.0f, 21.0f / 30.0f,
        8.0f / 8.0f, 25.0f / 30.0f,
        7.0f / 8.0f, 25.0f / 30.0f,
        0.0f / 8.0f, 16.0f / 30.0f,
        1.0f / 8.0f, 16.0f / 30.0f,
        1.0f / 8.0f, 20.0f / 30.0f,
        0.0f / 8.0f, 20.0f / 30.0f,
        1.0f / 8.0f, 16.0f / 30.0f,
        2.0f / 8.0f, 16.0f / 30.0f,
        2.0f / 8.0f, 20.0f / 30.0f,
        1.0f / 8.0f, 20.0f / 30.0f,
        2.0f / 8.0f, 16.0f / 30.0f,
        3.0f / 8.0f, 16.0f / 30.0f,
        3.0f / 8.0f, 20.0f / 30.0f,
        2.0f / 8.0f, 20.0f / 30.0f,
        3.0f / 8.0f, 16.0f / 30.0f,
        4.0f / 8.0f, 16.0f / 30.0f,
        4.0f / 8.0f, 20.0f / 30.0f,
        3.0f / 8.0f, 20.0f / 30.0f,
        4.0f / 8.0f, 16.0f / 30.0f,
        5.0f / 8.0f, 16.0f / 30.0f,
        5.0f / 8.0f, 20.0f / 30.0f,
        4.0f / 8.0f, 20.0f / 30.0f,
        5.0f / 8.0f, 16.0f / 30.0f,
        6.0f / 8.0f, 16.0f / 30.0f,
        6.0f / 8.0f, 20.0f / 30.0f,
        5.0f / 8.0f, 20.0f / 30.0f,
        6.0f / 8.0f, 16.0f / 30.0f,
        7.0f / 8.0f, 16.0f / 30.0f,
        7.0f / 8.0f, 20.0f / 30.0f,
        6.0f / 8.0f, 20.0f / 30.0f,
        7.0f / 8.0f, 16.0f / 30.0f,
        8.0f / 8.0f, 16.0f / 30.0f,
        8.0f / 8.0f, 20.0f / 30.0f,
        7.0f / 8.0f, 20.0f / 30.0f,
        0.0f / 8.0f, 11.0f / 30.0f,
        1.0f / 8.0f, 11.0f / 30.0f,
        1.0f / 8.0f, 15.0f / 30.0f,
        0.0f / 8.0f, 15.0f / 30.0f,
        1.0f / 8.0f, 11.0f / 30.0f,
        2.0f / 8.0f, 11.0f / 30.0f,
        2.0f / 8.0f, 15.0f / 30.0f,
        1.0f / 8.0f, 15.0f / 30.0f,
        2.0f / 8.0f, 11.0f / 30.0f,
        3.0f / 8.0f, 11.0f / 30.0f,
        3.0f / 8.0f, 15.0f / 30.0f,
        2.0f / 8.0f, 15.0f / 30.0f,
        3.0f / 8.0f, 11.0f / 30.0f,
        4.0f / 8.0f, 11.0f / 30.0f,
        4.0f / 8.0f, 15.0f / 30.0f,
        3.0f / 8.0f, 15.0f / 30.0f,
        4.0f / 8.0f, 11.0f / 30.0f,
        5.0f / 8.0f, 11.0f / 30.0f,
        5.0f / 8.0f, 15.0f / 30.0f,
        4.0f / 8.0f, 15.0f / 30.0f,
        5.0f / 8.0f, 11.0f / 30.0f,
        6.0f / 8.0f, 11.0f / 30.0f,
        6.0f / 8.0f, 15.0f / 30.0f,
        5.0f / 8.0f, 15.0f / 30.0f,
        6.0f / 8.0f, 11.0f / 30.0f,
        7.0f / 8.0f, 11.0f / 30.0f,
        7.0f / 8.0f, 15.0f / 30.0f,
        6.0f / 8.0f, 15.0f / 30.0f,
        7.0f / 8.0f, 11.0f / 30.0f,
        8.0f / 8.0f, 11.0f / 30.0f,
        8.0f / 8.0f, 15.0f / 30.0f,
        7.0f / 8.0f, 15.0f / 30.0f,
        0.0f / 8.0f, 6.0f / 30.0f,
        1.0f / 8.0f, 6.0f / 30.0f,
        1.0f / 8.0f, 10.0f / 30.0f,
        0.0f / 8.0f, 10.0f / 30.0f,
        1.0f / 8.0f, 6.0f / 30.0f,
        2.0f / 8.0f, 6.0f / 30.0f,
        2.0f / 8.0f, 10.0f / 30.0f,
        1.0f / 8.0f, 10.0f / 30.0f,
        2.0f / 8.0f, 6.0f / 30.0f,
        3.0f / 8.0f, 6.0f / 30.0f,
        3.0f / 8.0f, 10.0f / 30.0f,
        2.0f / 8.0f, 10.0f / 30.0f,
        3.0f / 8.0f, 6.0f / 30.0f,
        4.0f / 8.0f, 6.0f / 30.0f,
        4.0f / 8.0f, 10.0f / 30.0f,
        3.0f / 8.0f, 10.0f / 30.0f,
        4.0f / 8.0f, 6.0f / 30.0f,
        5.0f / 8.0f, 6.0f / 30.0f,
        5.0f / 8.0f, 10.0f / 30.0f,
        4.0f / 8.0f, 10.0f / 30.0f,
        5.0f / 8.0f, 6.0f / 30.0f,
        6.0f / 8.0f, 6.0f / 30.0f,
        6.0f / 8.0f, 10.0f / 30.0f,
        5.0f / 8.0f, 10.0f / 30.0f,
        6.0f / 8.0f, 6.0f / 30.0f,
        7.0f / 8.0f, 6.0f / 30.0f,
        7.0f / 8.0f, 10.0f / 30.0f,
        6.0f / 8.0f, 10.0f / 30.0f,
        7.0f / 8.0f, 6.0f / 30.0f,
        8.0f / 8.0f, 6.0f / 30.0f,
        8.0f / 8.0f, 10.0f / 30.0f,
        7.0f / 8.0f, 10.0f / 30.0f,
        0.0f / 8.0f, 1.0f / 30.0f,
        1.0f / 8.0f, 1.0f / 30.0f,
        1.0f / 8.0f, 5.0f / 30.0f,
        0.0f / 8.0f, 5.0f / 30.0f,
        1.0f / 8.0f, 1.0f / 30.0f,
        2.0f / 8.0f, 1.0f / 30.0f,
        2.0f / 8.0f, 5.0f / 30.0f,
        1.0f / 8.0f, 5.0f / 30.0f,
        2.0f / 8.0f, 1.0f / 30.0f,
        3.0f / 8.0f, 1.0f / 30.0f,
        3.0f / 8.0f, 5.0f / 30.0f,
        2.0f / 8.0f, 5.0f / 30.0f,
        3.0f / 8.0f, 1.0f / 30.0f,
        4.0f / 8.0f, 1.0f / 30.0f,
        4.0f / 8.0f, 5.0f / 30.0f,
        3.0f / 8.0f, 5.0f / 30.0f,
        4.0f / 8.0f, 1.0f / 30.0f,
        5.0f / 8.0f, 1.0f / 30.0f,
        5.0f / 8.0f, 5.0f / 30.0f,
        4.0f / 8.0f, 5.0f / 30.0f,
        5.0f / 8.0f, 1.0f / 30.0f,
        6.0f / 8.0f, 1.0f / 30.0f,
        6.0f / 8.0f, 5.0f / 30.0f,
        5.0f / 8.0f, 5.0f / 30.0f,
        6.0f / 8.0f, 1.0f / 30.0f,
        7.0f / 8.0f, 1.0f / 30.0f,
        7.0f / 8.0f, 5.0f / 30.0f,
        6.0f / 8.0f, 5.0f / 30.0f,
        7.0f / 8.0f, 1.0f / 30.0f,
        8.0f / 8.0f, 1.0f / 30.0f,
        8.0f / 8.0f, 5.0f / 30.0f,
        7.0f / 8.0f, 5.0f / 30.0f
    };

    auto TCloc = glGetUniformLocation(PlayerData.shader->GetRendererID(), "SpriteTCs");
    glUniform1fv(TCloc, 8 * 6 * 4 * 2, IdleTCs);

    auto Texloc = glGetUniformLocation(PlayerData.shader->GetRendererID(), "u_Textures");
    int TexIDs[1] = {7};
    glUniform1iv(Texloc, 1, TexIDs);

    PlayerData.SpriteSheet = std::make_unique<Texture>("res/sprites/PlayerSpriteSheet.png");
    glBindTextureUnit(7, PlayerData.SpriteSheet->GetRendererID());

    PreviousSpriteState = Idle;
    SpriteState = Idle;
}

Player::~Player()
{
    ResetInput();
}

void Player::Bind()
{
    PlayerData.VA->Bind();
    PlayerData.VB->Bind();
    PlayerData.IB->Bind();
    PlayerData.shader->Bind();
}

void Player::UpdateCurrentTiles(char bound, float x, float y)
{
    float LevelBaseX = x - WIDTH / 2;
    float LevelBaseY = y - HEIGHT / 2;

    if (LevelBaseX < 0 || LevelBaseX >= Chunks * ChunkWidth - WIDTH || LevelBaseY < 0 || LevelBaseY >= ChunkTilesY * TileSize - HEIGHT) {
        Death = true;
        return;
    }

    if (bound == Left) {
        unsigned int LeftChunk = int(LevelBaseX / ChunkWidth);
        float ChunkBaseX = LevelBaseX - LeftChunk * ChunkWidth;
        unsigned int TileInc = TileSize;

        unsigned int ChunkOffset = LeftChunk * ChunkSize;
        unsigned int ChunkTileOffsetX = int(ChunkBaseX / TileSize);
        CurrentTiles[0] = ChunkOffset + ChunkTileOffsetX + int(LevelBaseY / TileSize) * ChunkTilesX;
        CurrentTiles[3] = ChunkOffset + ChunkTileOffsetX + int((LevelBaseY + TileInc) / TileSize) * ChunkTilesX;
        CurrentTiles[5] = ChunkOffset + ChunkTileOffsetX + int((LevelBaseY + 2 * TileInc) / TileSize) * ChunkTilesX;
        CurrentTiles[7] = ChunkOffset + ChunkTileOffsetX + int((LevelBaseY + 3 * TileInc) / TileSize) * ChunkTilesX;
        CurrentTiles[9] = ChunkOffset + ChunkTileOffsetX + int((LevelBaseY + SizeFixY) / TileSize) * ChunkTilesX;
    }
    else if (bound == Right) {
        unsigned int RightChunk = int((LevelBaseX + SizeFixX) / ChunkWidth);
        float ChunkBaseX = LevelBaseX - RightChunk * ChunkWidth;
        unsigned int TileInc = TileSize;

        unsigned int ChunkOffset = RightChunk * ChunkSize;
        unsigned int ChunkTileOffsetX = int((ChunkBaseX + SizeFixX) / TileSize);
        CurrentTiles[2] = ChunkOffset + ChunkTileOffsetX + int(LevelBaseY / TileSize) * ChunkTilesX;
        CurrentTiles[4] = ChunkOffset + ChunkTileOffsetX + int((LevelBaseY + TileInc) / TileSize) * ChunkTilesX;
        CurrentTiles[6] = ChunkOffset + ChunkTileOffsetX + int((LevelBaseY + 2 * TileInc) / TileSize) * ChunkTilesX;
        CurrentTiles[8] = ChunkOffset + ChunkTileOffsetX + int((LevelBaseY + 3 * TileInc) / TileSize) * ChunkTilesX;
        CurrentTiles[11] = ChunkOffset + ChunkTileOffsetX + int((LevelBaseY + SizeFixY) / TileSize) * ChunkTilesX;
    }   
    else{
        unsigned int LeftChunk = int(LevelBaseX / ChunkWidth);
        unsigned int MidChunk = int((LevelBaseX + WIDTH/2) / ChunkWidth);
        unsigned int RightChunk = int((LevelBaseX + SizeFixX) / ChunkWidth);

        float ChunkBaseXL = LevelBaseX - LeftChunk * ChunkWidth;
        float ChunkBaseXM = LevelBaseX + WIDTH/2 - MidChunk * ChunkWidth;
        float ChunkBaseXR = LevelBaseX + SizeFixX - RightChunk * ChunkWidth;

        switch (bound) {
        case Bottom:
            CurrentTiles[0] = LeftChunk * ChunkSize + int(ChunkBaseXL / TileSize) + int(LevelBaseY / TileSize) * ChunkTilesX;
            CurrentTiles[1] = MidChunk * ChunkSize + int(ChunkBaseXM / TileSize) + int(LevelBaseY / TileSize) * ChunkTilesX;
            CurrentTiles[2] = RightChunk * ChunkSize + int(ChunkBaseXR / TileSize) + int(LevelBaseY / TileSize) * ChunkTilesX;
            break;
        case Top:
            CurrentTiles[9] = LeftChunk * ChunkSize + int(ChunkBaseXL / TileSize) + int((LevelBaseY + SizeFixY) / TileSize) * ChunkTilesX;
            CurrentTiles[10] = MidChunk * ChunkSize + int(ChunkBaseXM / TileSize) + int((LevelBaseY + SizeFixY) / TileSize) * ChunkTilesX;
            CurrentTiles[11] = RightChunk * ChunkSize + int(ChunkBaseXR / TileSize) + int((LevelBaseY + SizeFixY) / TileSize) * ChunkTilesX;
            break;
        }
    }
    
}

bool Player::TileCollision(char bound, bool ApproachingSlope, unsigned char TileCollider[Chunks * ChunkSize])
{
    bool Collision = false;

    if (!ApproachingSlope) {
        switch (bound) {
        case Left:
            Collision = TileCollider[CurrentTiles[0]] != 0 || TileCollider[CurrentTiles[3]] != 0 || TileCollider[CurrentTiles[5]] != 0 || TileCollider[CurrentTiles[7]] != 0 || TileCollider[CurrentTiles[9]] != 0;
            break;
        case Right:
            Collision = TileCollider[CurrentTiles[2]] != 0 || TileCollider[CurrentTiles[4]] != 0 || TileCollider[CurrentTiles[6]] != 0 || TileCollider[CurrentTiles[8]] != 0 || TileCollider[CurrentTiles[11]] != 0;
            break;
        case Bottom:
            Collision = (TileCollider[CurrentTiles[0]] != 0 && TileCollider[CurrentTiles[0]] < LowerSlopeBound) || (TileCollider[CurrentTiles[1]] != 0 && TileCollider[CurrentTiles[1]] < LowerSlopeBound) || (TileCollider[CurrentTiles[2]] != 0 && TileCollider[CurrentTiles[2]] < LowerSlopeBound);
            break;
        case Top:
            Collision = TileCollider[CurrentTiles[9]] != 0 || TileCollider[CurrentTiles[10]] != 0 || TileCollider[CurrentTiles[11]] != 0;
            break;
        }
    }
    else {
        switch (bound) {
        case Left:
            Collision = TileCollider[CurrentTiles[5]] != 0 || TileCollider[CurrentTiles[7]] != 0 || TileCollider[CurrentTiles[9]] != 0;
            break;
        case Right:
            Collision = TileCollider[CurrentTiles[6]] != 0 || TileCollider[CurrentTiles[8]] != 0 || TileCollider[CurrentTiles[11]] != 0;
            break;
        case Top:
            Collision = TileCollider[CurrentTiles[9]] != 0 || TileCollider[CurrentTiles[10]] != 0 || TileCollider[CurrentTiles[11]] != 0;
            break;
        }
    }

    return Collision;
}

bool Player::SlopeCollision(char bound, unsigned char TileCollider[Chunks * ChunkSize])
{
    bool Collision = false;

    switch (bound) {
    case Left:
        Collision = (TileCollider[CurrentTiles[0]] >= LowerSlopeBound && TileCollider[CurrentTiles[0]] <= UpperSlopeBound) || (TileCollider[CurrentTiles[3]] >= LowerSlopeBound && TileCollider[CurrentTiles[3]] <= UpperSlopeBound);
        break;
    case Right:
        Collision = (TileCollider[CurrentTiles[2]] >= LowerSlopeBound && TileCollider[CurrentTiles[2]] <= UpperSlopeBound) || (TileCollider[CurrentTiles[4]] >= LowerSlopeBound && TileCollider[CurrentTiles[4]] <= UpperSlopeBound);
        break;
    case Bottom:
        Collision = (TileCollider[CurrentTiles[0]] >= LowerSlopeBound && TileCollider[CurrentTiles[0]] <= UpperSlopeBound) || (TileCollider[CurrentTiles[1]] >= LowerSlopeBound && TileCollider[CurrentTiles[1]] <= UpperSlopeBound) || (TileCollider[CurrentTiles[2]] >=LowerSlopeBound && TileCollider[CurrentTiles[2]] <= UpperSlopeBound);
        break;
    }

    return Collision;
}

void Player::HandleInput(unsigned int FrameSteps)
{
    if (input.left == true) {
        if (Slowed == true) {
            PlayerVelX = -3.5f;
        }
        else {
            if (input.PlayerOnGround)
                PlayerVelX += -1.0f * FrameSteps;
            else
                PlayerVelX > 0.0f ? PlayerVelX += -0.5f * FrameSteps : PlayerVelX -= 0.25f * FrameSteps;
        }
        PlayerData.shader->SetUniform1f("AnimDirection", 1);
    }
    else if (input.right == true) {
        if (Slowed == true) {
            PlayerVelX = 3.5f;
        }
        else {
            if (input.PlayerOnGround)
                PlayerVelX += 1.0f * FrameSteps;
            else
                PlayerVelX < 0.0f ? PlayerVelX += 0.5f * FrameSteps : PlayerVelX += 0.25f * FrameSteps;
        }
        PlayerData.shader->SetUniform1f("AnimDirection", 0);
    }
    else {
        if (PlayerVelX != 0.0f) {
            if (PlayerVelX < 0 && PlayerVelX + 2.0f * FrameSteps > 0) {
                PlayerVelX = 0;
                if (input.PlayerOnGround && PreviousSpriteState != Landing) SpriteState = Idle;
            }
            else if (PlayerVelX > 0 && PlayerVelX - 2.0f * FrameSteps < 0) {
                PlayerVelX = 0;
                if (input.PlayerOnGround && PreviousSpriteState != Landing) SpriteState = Idle;
            }
            else {
                if (input.PlayerOnGround)
                    (PlayerVelX > 0.0f ? PlayerVelX += -2.0f * FrameSteps : PlayerVelX += 2.0f * FrameSteps);
                else
                    (PlayerVelX > 0.0f ? PlayerVelX += -0.5f * FrameSteps : PlayerVelX += 0.5f * FrameSteps);
            }
        }
    }

    if (PlayerVelX > 12)
        PlayerVelX = 12;
    if (PlayerVelX < -12)
        PlayerVelX = -12;

    Slowed = false;

    if (input.PlayerOnGround && (PlayerVelX == 12 || PlayerVelX == -12) && PreviousSpriteState != Landing)
        SpriteState = Running;
    else if (input.PlayerOnGround && (PlayerVelX < 12 || PlayerVelX > -12) && PlayerVelX != 0 && PreviousSpriteState != Landing)
        SpriteState = Walking;

    if (input.PlayerOnGround && PlayerVelX == 0 && PreviousSpriteState != Landing) SpriteState = Idle;

    if (input.jump != true) {
        JumpTracker = 0;
    }

    if (input.jump == true && (input.PlayerOnGround || (JumpTracker != 0))) {
        if (JumpTracker >= 10) {
            input.jump = false;
            JumpTracker = 0;
        }
        else {
            PlayerVelY = 15;
            JumpTracker += 1;
            input.PlayerOnGround = false;
            SpriteState = Jumping;
        }
    }
    else if (input.WallSliding && input.jump) {
        input.WallSliding = false;
        input.jump = false;
        PlayerVelY = 20;
        if (input.left) {
            PlayerVelX = 8.0f;
            PlayerData.shader->SetUniform1f("AnimDirection", 0);
        }
        else {
            PlayerVelX = -8.0f;
            PlayerData.shader->SetUniform1f("AnimDirection", 1);
        }
        SpriteState = Jumping;
    }
    else if (input.WallSliding && (input.left || input.right)) {
        PlayerVelY -= 0.5f * FrameSteps;
        input.WallSliding = false;
        SpriteState = WallSlide;
    }
    else {
        PlayerVelY -= 0.75f * FrameSteps;
        input.WallSliding = false;
        input.jump = false;
    }

    if (!input.PlayerOnGround && SpriteState != Jumping && SpriteState != WallSlide && ApproachingSlope == false)
        SpriteState = Falling;

    if (PlayerVelY < -13)
        PlayerVelY = -13;
}

void Player::UpdateSprite(unsigned int FrameSteps)
{
    if (!OnSlope && CurrentRotation != 0) {
        model = glm::rotate(model, -CurrentRotation, glm::vec3(0, 0, 1));
        PlayerData.shader->SetUniformMat4f("u_MVP", model * view * proj);
        CurrentRotation = 0;
    }

    FrameCounter += FrameSteps;

    if (SpriteState != PreviousSpriteState) {
        AnimFrame = 0;
        FrameCounter = 0;
    }

    switch (SpriteState) {
    case Idle:
        if (FrameCounter >= 6) {
            PlayerData.shader->SetUniform1f("AnimFrame", AnimFrame);
            AnimFrame >= 9 ? AnimFrame = 0 : AnimFrame += 1;
            FrameCounter -= 6;
        }
        PlayerData.shader->SetUniform1f("AnimOffset", 0);
        break;

    case Jumping:
        if (PlayerVelY > 3)
            AnimFrame = 0;
        else if (PlayerVelY < -3)
            AnimFrame = 2;
        else
            AnimFrame = 1;
        PlayerData.shader->SetUniform1f("AnimFrame", AnimFrame);
        PlayerData.shader->SetUniform1f("AnimOffset", 10 * 4 * 2);
        break;

    case WallSlide:
        if (PlayerVelY > -3)
            AnimFrame = 0;
        else if (PlayerVelY > -7)
            AnimFrame = 1;
        else
            AnimFrame = 2;
        PlayerData.shader->SetUniform1f("AnimFrame", AnimFrame);
        PlayerData.shader->SetUniform1f("AnimOffset", 13 * 4 * 2);
        if (input.WallSliding == false)
            SpriteState = Falling;
        break;

    case Walking:
        if (FrameCounter >= 4) {
            PlayerData.shader->SetUniform1f("AnimFrame", AnimFrame);
            AnimFrame >= 7 ? AnimFrame = 0 : AnimFrame += 1;
            FrameCounter -= 4;
        }
        PlayerData.shader->SetUniform1f("AnimOffset", 16 * 4 * 2);
        break;

    case Running:
        if (FrameCounter >= 4) {
            PlayerData.shader->SetUniform1f("AnimFrame", AnimFrame);
            AnimFrame >= 7 ? AnimFrame = 0 : AnimFrame += 1;
            FrameCounter -= 4;
        }
        PlayerData.shader->SetUniform1f("AnimOffset", 24 * 4 * 2);
        break;

    case Landing:
        if (FrameCounter >= 3) {
            PlayerData.shader->SetUniform1f("AnimFrame", AnimFrame);
            if (AnimFrame >= 8) {
                SpriteState = Idle;
            }
            else if (input.PlayerOnGround && (PlayerVelX == 12 || PlayerVelX == -12))
                SpriteState = Running;
            else if (input.PlayerOnGround && ((PlayerVelX < 12 && PlayerVelX > 3) || (PlayerVelX > -12 && PlayerVelX < -3)) && PlayerVelX != 0)
                SpriteState = Walking;
            else
                AnimFrame += 1;
            FrameCounter -= 3;
        }
        PlayerData.shader->SetUniform1f("AnimOffset", 32 * 4 * 2);
        break;

    case Falling:
        AnimFrame = 2;
        PlayerData.shader->SetUniform1f("AnimFrame", AnimFrame);
        PlayerData.shader->SetUniform1f("AnimOffset", 10 * 4 * 2);
        break;
    }

    PreviousSpriteState = SpriteState;
}


bool Player::CheckIfOnSlope(float x, float& y, unsigned char TileCollider[Chunks * ChunkSize], unsigned char SlopeHeights[12 * 2])
{
    
    float LevelBaseY = y - HEIGHT / 2;
    unsigned int CurrentChunk = int(x / ChunkWidth);
    float ChunkBaseX = x - CurrentChunk * ChunkWidth;
    unsigned int CurrentTile = CurrentChunk * ChunkSize + int(ChunkBaseX / TileSize) + int(LevelBaseY / TileSize) * ChunkTilesX;

    unsigned int SlopeFinder = CurrentTile - ChunkTilesX;
    bool SlopeFound = false;

    for (SlopeFinder; SlopeFinder <= CurrentTile + ChunkTilesX; SlopeFinder += ChunkTilesX) {
        int ChunkPosY = int((SlopeFinder % ChunkSize) / ChunkTilesX);
        if (ChunkPosY > 48 || ChunkPosY < -1)
            continue;
        if (TileCollider[SlopeFinder] >= LowerSlopeBound && TileCollider[SlopeFinder] <= UpperSlopeBound) {
            SlopeFound = true;
            break;
        }
    }

    if (input.PlayerOnGround && SlopeFound)
        return true;
    else if (SlopeFound) {
        // Could make this a function
        unsigned int SlopeBlockType = TileCollider[SlopeFinder];
        if (SlopeBlockType >= LowerSlopeBound && SlopeBlockType <= UpperSlopeBound) {
            unsigned int SlopeHeightIndex = (SlopeBlockType - LowerSlopeBound) * 2;
            float SlopeHeightL = SlopeHeights[SlopeHeightIndex];
            float SlopeHeightR = SlopeHeights[SlopeHeightIndex + 1];

            float SlopePosX = x - (((SlopeFinder % ChunkSize) % ChunkTilesX) * TileSize + int(SlopeFinder / ChunkSize) * ChunkWidth);

            float SlopeTravelled = SlopePosX / TileSize;
            float SlopePosY = (1 - SlopeTravelled) * SlopeHeightL + SlopeTravelled * SlopeHeightR;

            float SlopeBasedPlayerY = int((SlopeFinder - CurrentChunk * ChunkSize) / ChunkTilesX) * TileSize + SlopePosY + HEIGHT / 2;

            if (y < SlopeBasedPlayerY)
                return true;
        }
    }
    
    return false;
}

void Player::HandleSlopeCollision(float x, float& y, unsigned char TileCollider[Chunks * ChunkSize], unsigned char SlopeHeights[12 * 2])
{
    float LevelBaseY = y - HEIGHT / 2;
    unsigned int CurrentChunk = int(x / ChunkWidth);
    float ChunkBaseX = x - CurrentChunk * ChunkWidth;
    unsigned int CurrentTile = CurrentChunk * ChunkSize + int(ChunkBaseX / TileSize) + int(LevelBaseY / TileSize) * ChunkTilesX;

    unsigned int SlopeFinder = CurrentTile - ChunkTilesX;

    for (SlopeFinder; SlopeFinder <= CurrentTile + ChunkTilesX; SlopeFinder += ChunkTilesX) {
        int ChunkPosY = int((SlopeFinder % ChunkSize) / ChunkTilesX);
        if (ChunkPosY > 47 || ChunkPosY < -1)
            continue;
        if (TileCollider[SlopeFinder] >= LowerSlopeBound && TileCollider[SlopeFinder] <= UpperSlopeBound) {
            break;
        }
    }

    // Could make this a function
    unsigned int SlopeBlockType = TileCollider[SlopeFinder];
    if (SlopeBlockType >= LowerSlopeBound && SlopeBlockType <= UpperSlopeBound) {
        unsigned int SlopeHeightIndex = (SlopeBlockType - LowerSlopeBound) * 2;
        float SlopeHeightL = SlopeHeights[SlopeHeightIndex];
        float SlopeHeightR = SlopeHeights[SlopeHeightIndex + 1];

        float SlopePosX = x - (((SlopeFinder % ChunkSize) % ChunkTilesX) * TileSize + int(SlopeFinder / ChunkSize) * ChunkWidth);

        float SlopeTravelled = SlopePosX / TileSize;
        float SlopePosY = (1 - SlopeTravelled) * SlopeHeightL + SlopeTravelled * SlopeHeightR;

        float SlopeBasedPlayerY = int((SlopeFinder - CurrentChunk * ChunkSize) / ChunkTilesX) * TileSize + SlopePosY + HEIGHT/2;

        y = SlopeBasedPlayerY;

        if (SpriteState == Idle && CurrentRotation == 0) {
            float opp = 0;
            SlopeHeightL < SlopeHeightR ? opp = SlopeHeightR - SlopeHeightL + 1 : opp = SlopeHeightR - SlopeHeightL - 1;
            CurrentRotation = atanf(opp / 32.0f);
            model = glm::rotate(model, CurrentRotation, glm::vec3(0, 0, 1));
            PlayerData.shader->SetUniformMat4f("u_MVP", model * view * proj);
        }
    }
}

void Player::UpdatePos(unsigned char TileCollider[Chunks * ChunkSize], unsigned char SlopeHeights[12 * 2], std::unordered_map<unsigned int, Interactable*>& Entities, unsigned int FrameSteps)
{
    Bind();

    HandleInput(FrameSteps);

    float TileStepsX = PlayerVelX / TileSize;
    float TileStepsY = PlayerVelY / TileSize;

    float NewPlayerPosX = PlayerPosX;
    float NewPlayerPosY = PlayerPosY;

    if (PlayerVelX < 0) {
        for (float TileSteps = -1; TileSteps > TileStepsX - 1; TileSteps--) {
            NewPlayerPosX = PlayerPosX + (TileSteps > TileStepsX ? TileSteps * TileSize : TileStepsX * TileSize);
            UpdateCurrentTiles(Left, NewPlayerPosX, NewPlayerPosY);

            ApproachingSlope = (OnSlope ? true : false);
            OnSlope = CheckIfOnSlope(NewPlayerPosX, NewPlayerPosY, TileCollider, SlopeHeights);
            if (OnSlope || (!OnSlope && SlopeCollision(Left, TileCollider)))
                ApproachingSlope = true;
            if (OnSlope) {
                HandleSlopeCollision(NewPlayerPosX, NewPlayerPosY, TileCollider, SlopeHeights);
                UpdateCurrentTiles(Left, NewPlayerPosX, NewPlayerPosY);
            }

            if (TileCollision(Left, ApproachingSlope, TileCollider)) {
                unsigned int CurrentChunk = int(CurrentTiles[0] / ChunkSize);
                unsigned int CurrentTileX = (CurrentTiles[0] % ChunkSize) % ChunkTilesX;
                NewPlayerPosX = (CurrentTileX + 1) * TileSize + CurrentChunk * ChunkWidth + WIDTH / 2;
                if (OnSlope)
                    HandleSlopeCollision(NewPlayerPosX, NewPlayerPosY, TileCollider, SlopeHeights);
                if (!input.PlayerOnGround)
                    input.WallSliding = true;
                else
                    SpriteState = Idle;
                PlayerVelX = 0;
                break;
            }
            else
                HandleEntities(Left, Entities);
        }
    }
    else if (PlayerVelX > 0) {
        for (float TileSteps = 1; TileSteps < TileStepsX + 1; TileSteps++) {
            NewPlayerPosX = PlayerPosX + (TileSteps < TileStepsX ? TileSteps * TileSize : TileStepsX * TileSize);
            UpdateCurrentTiles(Right, NewPlayerPosX, NewPlayerPosY);

            ApproachingSlope = (OnSlope ? true : false);
            OnSlope = CheckIfOnSlope(NewPlayerPosX, NewPlayerPosY, TileCollider, SlopeHeights);
            if (OnSlope || (!OnSlope && SlopeCollision(Right, TileCollider)))
                ApproachingSlope = true;
            if (OnSlope) {
                HandleSlopeCollision(NewPlayerPosX, NewPlayerPosY, TileCollider, SlopeHeights);
                UpdateCurrentTiles(Right, NewPlayerPosX, NewPlayerPosY);
            }

            if (TileCollision(Right, ApproachingSlope, TileCollider)) {
                unsigned int CurrentChunk = int(CurrentTiles[2] / ChunkSize);
                unsigned int CurrentTileX = (CurrentTiles[2] % ChunkSize) % ChunkTilesX;
                NewPlayerPosX = CurrentTileX * TileSize + CurrentChunk * ChunkWidth - WIDTH / 2;
                if (OnSlope)
                    HandleSlopeCollision(NewPlayerPosX, NewPlayerPosY, TileCollider, SlopeHeights);
                if (!input.PlayerOnGround)
                    input.WallSliding = true;
                else
                    SpriteState = Idle;
                PlayerVelX = 0;
                break;
            }
            else
                HandleEntities(Right, Entities);
        }
    }

    if (PlayerVelY < 0) {
        PlayerPosY = NewPlayerPosY;
        if (OnSlope) {
            UpdateCurrentTiles(Bottom, NewPlayerPosX, NewPlayerPosY);
            HandleSlopeCollision(NewPlayerPosX, NewPlayerPosY, TileCollider, SlopeHeights);
            input.PlayerOnGround = true;
            PlayerVelY = 0.0f;
        }
        else {
            for (float TileSteps = -1; TileSteps > TileStepsY - 1; TileSteps--) {
                NewPlayerPosY = PlayerPosY + (TileSteps > TileStepsY ? TileSteps * TileSize : TileStepsY * TileSize);
                UpdateCurrentTiles(Bottom, NewPlayerPosX, NewPlayerPosY);
                OnSlope = CheckIfOnSlope(NewPlayerPosX, NewPlayerPosY, TileCollider, SlopeHeights);
                if (OnSlope) {
                    HandleSlopeCollision(NewPlayerPosX, NewPlayerPosY, TileCollider, SlopeHeights);
                    input.PlayerOnGround = true;
                    PlayerVelY = 0.0f;
                    break;
                }
                else {
                    ApproachingSlope = false;
                    if (SlopeCollision(Bottom, TileCollider))
                        (TileCollision(Bottom, ApproachingSlope, TileCollider) ? ApproachingSlope = false : ApproachingSlope = true);
                    if (TileCollision(Bottom, ApproachingSlope, TileCollider)) {
                        unsigned int CurrentTileY = int((CurrentTiles[0] % ChunkSize) / ChunkTilesX);
                        NewPlayerPosY = (CurrentTileY + 1) * TileSize + HEIGHT / 2;
                        input.PlayerOnGround = true;
                        PlayerVelY = 0.0f;
                        if ((PreviousSpriteState == Jumping || PreviousSpriteState == Falling) && (PlayerVelX > -2 || PlayerVelX < 2)) SpriteState = Landing;
                        break;
                    }
                    else {
                        HandleEntities(Bottom, Entities);
                        input.PlayerOnGround = false;
                    }
                }
            }
        }
    }
    else if (PlayerVelY > 0) {
        input.PlayerOnGround = false;
        OnSlope = false;
        for (float TileSteps = 1; TileSteps < TileStepsY + 1; TileSteps++) {
            NewPlayerPosY = PlayerPosY + (TileSteps < TileStepsY ? TileSteps * TileSize : TileStepsY * TileSize);
            UpdateCurrentTiles(Top, NewPlayerPosX, NewPlayerPosY);
            if (TileCollision(Top, ApproachingSlope, TileCollider)) {
                unsigned int CurrentTileY = int((CurrentTiles[9] % ChunkSize) / ChunkTilesX);
                NewPlayerPosY = CurrentTileY * TileSize - HEIGHT / 2;
                PlayerVelY = 0.0f;
                break;
            }
            else
                HandleEntities(Top, Entities);
        }
    }

    PlayerPosX = NewPlayerPosX;
    PlayerPosY = NewPlayerPosY;

    UpdateSprite(FrameSteps);
}

void Player::HandleEntities(char bound, std::unordered_map<unsigned int, Interactable*>& Entities)
{
    int start = 0, end = 0, Inc = 0, Anom = 0, Fix = 0;

    switch (bound) {
    case Left:
        start = 0; end = 10;  Inc = 2; Anom = 2; Fix = 3;
        break;
    case Right:
        start = 2; end = 12; Inc = 2; Anom = 10; Fix = 11;
        break;
    case Bottom:
        start = 0; end = 3; Inc = 1; Anom = -1; Fix = -1;
        break;
    case Top:
        start = 9; end = 12; Inc = 1; Anom = -1; Fix = -1;
        break;
    }

    for (int t = start; t < end; t += Inc) {
        if (t == Anom) t = Fix;

        auto EntFinder = Entities.find(CurrentTiles[t]);
        if (EntFinder != Entities.end()) {
            auto* Ent = EntFinder->second;
            if (Ent->GetEntID() <= 4) {
                Death = true;
                break;
            }
            else if (input.PlayerOnGround == true) {
                Slowed = true;
                break;
            }
        }
    }
}

void Player::Draw()
{
    Bind();
    if (Death)
        DeathFade -= 0.05f;
    if (DeathFade < 0.0f)
        DeathFade = 0.0f;

    PlayerData.shader->SetUniform1f("DeathFade", DeathFade);
    PlayerData.shader->SetUniformMat4f("u_MVP", proj * view * model);

    Renderer::Draw(PlayerData.VA, PlayerData.IB, PlayerData.shader);
}