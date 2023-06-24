#include "Wall.h"

Wall::Wall()
{
    WallData.VA = std::make_unique<VertexArray>();
    WallData.VA->Bind();

    float WallPositions[] = {
        -2120.0f,    0.0f, 8.0f,
         -200.0f,    0.0f, 8.0f,
         -200.0f, 1536.0f, 8.0f,
        -2120.0f, 1536.0f, 8.0f,
         -200.0f,    0.0f, 9.0f,
            0.0f,    0.0f, 9.0f,
            0.0f, 1536.0f, 9.0f,
         -200.0f, 1536.0f, 9.0f,
    };

    WallData.VB = std::make_unique<VertexBuffer>(WallPositions, sizeof(WallPositions));
    WallData.VB->Bind();

    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(1);

    WallData.VA->AddBuffer(*WallData.VB, layout);

    unsigned int* indices = new unsigned int[12];
    unsigned int offset = 0;
    for (size_t i = 0; i < 12; i += 6)
    {
        indices[i + 0] = 0 + offset;
        indices[i + 1] = 1 + offset;
        indices[i + 2] = 2 + offset;

        indices[i + 3] = 2 + offset;
        indices[i + 4] = 3 + offset;
        indices[i + 5] = 0 + offset;

        offset += 4;
    }

    WallData.IB = std::make_unique<IndexBuffer>(indices, 12);
    WallData.IB->Bind();

    delete[] indices;
    indices = nullptr;

    WallData.shader = std::make_unique<Shader>("res/shaders/Wall.shader");
    WallData.shader->Bind();
    WallData.shader->SetUniformMat4f("u_MVP", proj * view * model);

    auto Texloc = glGetUniformLocation(WallData.shader->GetRendererID(), "u_Textures");
    int TexIDs[2];
    for (int i = 0; i < 2; i++)
        TexIDs[i] = i + 8;
    glUniform1iv(Texloc, 2, TexIDs);

    WallData.WallBase = std::make_unique<Texture>("res/textures/WallBase.png");
    WallData.WallFront = std::make_unique<Texture>("res/textures/WallFront.png");

    glBindTextureUnit(8, WallData.WallBase->GetRendererID());
    glBindTextureUnit(9, WallData.WallFront->GetRendererID());

    WallData.shader->SetUniform1f("Frame", 0.0f);

    float FrameInfo[4 * 4 * 2] = {
        1.0f / 20.0f, 40.0f / 1616.0f,
        5.0f / 20.0f, 40.0f / 1616.0f,
        5.0f / 20.0f, 1576.0f / 1616.0f,
        1.0f / 20.0f, 1576.0f / 1616.0f,
        6.0f / 20.0f, 40.0f / 1616.0f,
        10.0f / 20.0f, 40.0f / 1616.0f,
        10.0f / 20.0f, 1576.0f / 1616.0f,
        6.0f / 20.0f, 1576.0f / 1616.0f,
        11.0f / 20.0f, 40.0f / 1616.0f,
        15.0f / 20.0f, 40.0f / 1616.0f,
        15.0f / 20.0f, 1576.0f / 1616.0f,
        11.0f / 20.0f, 1576.0f / 1616.0f,
        16.0f / 20.0f, 40.0f / 1616.0f,
        20.0f / 20.0f, 40.0f / 1616.0f,
        20.0f / 20.0f, 1576.0f / 1616.0f,
        16.0f / 20.0f, 1576.0f / 1616.0f
    };

    int FrameInfoloc = glGetUniformLocation(WallData.shader->GetRendererID(), "FrameInfo");
    glUniform1fv(FrameInfoloc, 4 * 4 * 2, FrameInfo);
}

Wall::~Wall()
{
}

void Wall::Bind()
{
    WallData.VA->Bind();
    WallData.VB->Bind();
    WallData.IB->Bind();
    WallData.shader->Bind();
}

void Wall::Draw()
{
    Bind();
    WallData.shader->SetUniform1f("Frame", (float)FrontState);
    WallData.shader->SetUniformMat4f("u_MVP", proj * view * model);
    Renderer::Draw(WallData.VA, WallData.IB, WallData.shader);
}
