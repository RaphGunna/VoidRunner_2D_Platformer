#include "Renderer.h"

void Renderer::Clear()
{
    GLCall(glClearColor(0.5f, 0.5f, 0.5f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const std::unique_ptr<VertexArray>& va, const std::unique_ptr<IndexBuffer>& ib, const std::unique_ptr<Shader>& shader)
{
    shader->Bind();
    va->Bind();
    ib->Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr));
}