#include "gravpch.h"
#include "normal.h"
#include <math.h>

#define EPSILON 1e-7

GRAVEngine::AI::Distributions::normal::normal(torch::Tensor location, torch::Tensor scale) : distribution(location.sizes()), m_Location(location), m_Scale(scale)
{
}

torch::Tensor GRAVEngine::AI::Distributions::normal::mean()
{
    return m_Location;
}

torch::Tensor GRAVEngine::AI::Distributions::normal::variance()
{
    return stddev().pow(2);
}

torch::Tensor GRAVEngine::AI::Distributions::normal::stddev()
{
    return m_Scale;
}

torch::Tensor GRAVEngine::AI::Distributions::normal::sample(c10::IntArrayRef sampleShape)
{
    // Get the extended shape
    auto shape = _extended_shape(sampleShape);
    c10::IntArrayRef shapeArray = c10::ArrayRef<int64_t>(shape);

    // Return a new tensor using normal from the expanded shape
    torch::NoGradGuard no_grad;
    return at::normal(m_Location.expand(shapeArray), m_Scale.expand(shapeArray));
}

torch::Tensor GRAVEngine::AI::Distributions::normal::logProb(torch::Tensor value)
{
    torch::Tensor logScale = torch::log(m_Scale + EPSILON);
    return -(
        torch::pow(value - m_Location, 2) / (2 * variance() + EPSILON) - logScale - log(sqrt(2 * M_PI))
        );
}

torch::Tensor GRAVEngine::AI::Distributions::normal::entropy()
{
    return 0.5 + 0.5 * log(2 * M_PI * std::exp(1) * m_Scale + EPSILON).mean();
}


