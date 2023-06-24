#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#include <memory>

class Renderer
{
public:
    static void Clear();
    static void Draw(const std::unique_ptr<VertexArray>& va, const std::unique_ptr<IndexBuffer>& ib, const std::unique_ptr<Shader>& shader);
};