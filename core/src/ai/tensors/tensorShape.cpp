#include "gravpch.h"
#include "tensorShape.h"

GRAVEngine::AI::Tensors::tensorShape::tensorShape(size_t n) : tensorShape(n, 1, 1, 1){}
GRAVEngine::AI::Tensors::tensorShape::tensorShape(size_t n, size_t c) : tensorShape(n, 1, 1, c){}
GRAVEngine::AI::Tensors::tensorShape::tensorShape(size_t n, size_t w, size_t c) : tensorShape(n, 1, w, c){}
GRAVEngine::AI::Tensors::tensorShape::tensorShape(size_t n, size_t h, size_t w, size_t c)
{
    m_Batch = n > 0 ? n : 1;
    m_Height = h > 0 ? h : 1;
    m_Width = w > 0 ? w : 1;
    m_Channels = c > 0 ? c : 1;
}



size_t GRAVEngine::AI::Tensors::tensorShape::operator[](size_t axis)
{
    switch (axis)
    {
    case 0:
        return m_Batch;
    case 1:
        return m_Height;
    case 2:
        return m_Width;
    case 3:
    default:
        return m_Channels;
    }
}

size_t GRAVEngine::AI::Tensors::tensorShape::index(size_t n, size_t h, size_t w, size_t c)
{
    return
        n * m_Height * m_Width * m_Channels +
        h * m_Width * m_Channels +
        w * m_Channels +
        c;
}

size_t GRAVEngine::AI::Tensors::tensorShape::index(size_t n, size_t c)
{
    return n * flatWidth() + c;
}

GRAVEngine::AI::Tensors::tensorShape GRAVEngine::AI::Tensors::tensorShape::flatten()
{
    return tensorShape(m_Batch, m_Channels * m_Height * m_Width);
}
