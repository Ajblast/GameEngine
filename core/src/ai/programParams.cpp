#include "gravpch.h"
#include "programParams.h"
#include "ai/agentPrograms/inferenceProgram.h"
#include "ai/agentPrograms/trainingProgram.h"

GRAVEngine::AI::programParams::programParams(inferenceDevice device, const std::string& name, ref<Models::model> model) : m_Model(model), m_InferenceDevice(device), m_Name(name)
{
}

GRAVEngine::scope<GRAVEngine::AI::IAgentProgram> GRAVEngine::AI::programParams::createProgram(const std::vector<ref<Sensors::ISensor>>& sensors, Actions::actuatorList& actuators)
{
    scope<IAgentProgram> program;
    if (m_Model == nullptr)
    {
        program = createScope<trainingProgram>(m_Name, sensors, actuators);
    }
    else
    {
        program = createScope<inferenceProgram>(m_Model);
    }

    program->sendToDevice(m_InferenceDevice);

    return program;
}
