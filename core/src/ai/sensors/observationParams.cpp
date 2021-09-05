#include "gravpch.h"
#include "observationParams.h"

GRAVEngine::AI::Sensors::observationParams::observationParams(size_t length) : observationParams(Tensors::tensorShape(length))
{
}

GRAVEngine::AI::Sensors::observationParams::observationParams(GRAVEngine::AI::Tensors::tensorShape shape) : m_Shape(shape)
{
}
