#include "gravpch.h"
#include "vectorSensor.h"

GRAVEngine::AI::Sensors::vectorSensor::vectorSensor(uint32 obsSize, std::string name) :
    m_Name(name), m_ObservationParams(obsSize), m_Values(createScope<float[]>(obsSize))
{
}

GRAVEngine::AI::Sensors::vectorSensor::~vectorSensor()
{
}

size_t GRAVEngine::AI::Sensors::vectorSensor::write(observationWriter& writer)
{
    // Get the number of observations
    size_t expectedObservations = m_ObservationParams.m_Shape[0];

    // Log a warning if the amount of observations was less than what was expected
    if (m_CurrentIndex < m_ObservationParams.m_Shape[0] - 1)
        GRAV_LOG_LINE_WARN("Less observations given than expected for vector sensor.");

    // Write to the writer
    writer.write(m_Values.get(), m_ObservationParams.m_Shape[0]);

    // Return the number of values written
    return m_ObservationParams.m_Shape[0];
}

void GRAVEngine::AI::Sensors::vectorSensor::update()
{
    // Clear the values buffer
    clear();
}

void GRAVEngine::AI::Sensors::vectorSensor::reset()
{
    // Clear the values buffer
    clear();
}

GRAVEngine::AI::Sensors::observationParams GRAVEngine::AI::Sensors::vectorSensor::getObservationParams()
{
    return m_ObservationParams;
}

std::string GRAVEngine::AI::Sensors::vectorSensor::getName()
{
    return m_Name;
}

void GRAVEngine::AI::Sensors::vectorSensor::addObservation(float obs)
{
    if (m_CurrentIndex >= m_ObservationParams.m_Shape.length())
        return;

    // Add an observation to the buffer
    m_Values[m_CurrentIndex++] = obs;
}

void GRAVEngine::AI::Sensors::vectorSensor::clear()
{
    // Set all the values of the vector sensor to 0
    memset(m_Values.get(), 0, m_ObservationParams.m_Shape.length() * sizeof(float));
}
