#include "gravpch.h"
#include "agent.h"
#include "exceptions/exceptions.h"
#include "ai/episodeCounter.h"

#include <algorithm>
#include <functional>

GRAVEngine::AI::agent::agent(programParams programParams, size_t maxStep) :
    m_Brain(nullptr), m_Info(), m_Actuators(), m_Sensors(),
    m_ShouldRequestAction(false), m_ShouldRequestDecision(false), m_Initialized(false), m_AgentParams(), m_BehaviorParams(programParams),
    m_StepHandle(), m_SendInfoHandle(), m_DecideActionHandle(), m_ActHandle(), m_ResetHandle()
{
    m_AgentParams.m_MaxStepCount = maxStep;
}
GRAVEngine::AI::agent::agent(std::vector<ref<Sensors::ISensor>>& sensors, std::vector<ref<Actions::IActuator>>& actuators, programParams programParams, size_t maxStep) :
    m_Brain(nullptr), m_Info(), m_Actuators(), m_Sensors(),
    m_ShouldRequestAction(false), m_ShouldRequestDecision(false), m_Initialized(false), m_AgentParams(), m_BehaviorParams(programParams),
    m_StepHandle(), m_SendInfoHandle(), m_DecideActionHandle(), m_ActHandle(), m_ResetHandle()
{
    m_AgentParams.m_MaxStepCount = maxStep;

    {
        GRAV_PROFILE_SCOPE("Initialize Actuators");
        initializeActuators(actuators);
    }
    
    {
        GRAV_PROFILE_SCOPE("Initialize Sensors");
        initializeSensors(m_Sensors);
    }

    // This constructor will initialize the agent
    safeInitialization();
}

GRAVEngine::AI::agent::~agent()
{
}

void GRAVEngine::AI::agent::markDone(doneReason reason)
{
    GRAV_PROFILE_FUNCTION();
    
    // Do nothing if the agent is already done.
    if (m_Info.m_IsDone)
        return;

    m_Info.m_IsDone = true;
    m_Info.m_maxStepReached = reason == doneReason::MAX_STEP_REACHED;

    updateSensors();

    // Collect the last observations into the abstract vector sensor
    //collectObservations(m_VectorSensor);

    // Create a temporary buffer for the sensors that is a scope instead of dynamic vector
    scope<ref<Sensors::ISensor> []> sensors = createScope<ref<Sensors::ISensor> []>(m_Sensors.size());
    for (size_t i = 0; i < m_Sensors.size(); i++)
        sensors[i] = m_Sensors[i];
    
    // Copy the data into the buffer
    //memcpy(sensors.get(), m_Sensors.data(), m_Sensors.size() * sizeof(ref<Sensors::ISensor>));

    // Request the final decision from the brain
    if (m_Brain != nullptr)
        m_Brain->requestDecision(m_Info, sensors, m_Sensors.size());

    // Reset the sensors afterwards
    resetSensors();


    // Null the reset of the data
    m_Info.m_Reward = 0;
    m_Info.m_CumulativeReward = 0;
    m_ShouldRequestDecision = false;
    m_ShouldRequestAction = false;
    m_Info.clearActions();
}

void GRAVEngine::AI::agent::initializeSensors(std::vector<ref<Sensors::ISensor>>& sensors)
{
    GRAV_PROFILE_FUNCTION();
    
    if (m_Initialized)
        return;

    // Add all of the sensors to the list of sensors
    for (auto it = sensors.begin(); it != sensors.end(); it++)
        m_Sensors.push_back(*it);

    // Add the potential vectorSensor
    //if (m_BehaviorParams.m_BrainParams.vectorObsSize > 0)
    //    m_Sensors.push_back(m_VectorSensor);

    // Sort the sensors by name for determinism
    sortSensors();

    // Make sure that no two sensors have the same name
    validateSensors();
}
void GRAVEngine::AI::agent::initializeActuators(std::vector<ref<Actions::IActuator>>& actuators)
{
    GRAV_PROFILE_FUNCTION();
    
    if (m_Initialized)
        return;

    // Add all of the actuators to the list of actuators
    for (auto it = actuators.begin(); it != actuators.end(); it++)
        m_Actuators.addActuator(*it);
}

void GRAVEngine::AI::agent::sendInfoToBrain()
{
    GRAV_PROFILE_FUNCTION();
   
    if (m_Initialized == false)
        throw Exceptions::agentException("sendInfoToBrain called while agent isn't initialized");

    if (m_Brain == nullptr)
        return;

    // Copy the infomation if it is not done. Clear for empty if not.
    if (m_Info.m_IsDone)
        m_Info.clearActions();
    else
        m_Info.copyActions(m_Actuators.buffer());

    // Update the sensors and collect observations
    updateSensors();
    //{
    //    GRAV_PROFILE_SCOPE("Collect Observations");
    //    collectObservations(m_VectorSensor);
    //}

    m_Info.m_IsDone = false;
    m_Info.m_maxStepReached = false;


    // Create a temporary buffer for the sensors that is a scope instead of dynamic vector
    scope<ref<Sensors::ISensor> []> sensors = createScope<ref<Sensors::ISensor>[]>(m_Sensors.size());
    for (size_t i = 0; i < m_Sensors.size(); i++)
        sensors[i] = m_Sensors[i];
    
    // Copy the data into the buffer
    //memcpy(sensors.get(), m_Sensors.data(), m_Sensors.size() * sizeof(ref<Sensors::ISensor>));

    {
        GRAV_PROFILE_SCOPE("Request Decision");
        // Request Decision
        m_Brain->requestDecision(m_Info, sensors, m_Sensors.size());
    }
}

void GRAVEngine::AI::agent::sortSensors()
{
    // Sort the actuators by name
    std::sort(m_Sensors.begin(), m_Sensors.end(), [](ref<Sensors::ISensor> lhs, ref<Sensors::ISensor> rhs) {return lhs->getName() < rhs->getName(); });
}
void GRAVEngine::AI::agent::validateSensors()
{
    for (size_t i = 0; i < m_Sensors.size() - 1; i++)
        GRAV_ASSERT(m_Sensors[i]->getName().compare(m_Sensors[i + 1]->getName()) != 0);
}

void GRAVEngine::AI::agent::setReward(float reward)
{
    m_Info.m_CumulativeReward += (reward - m_Info.m_Reward);    // Cumulative reward changes in the direction of the set reward
    m_Info.m_Reward = reward;                                   // Set the current reward
}
void GRAVEngine::AI::agent::addReward(float increment)
{
    m_Info.m_Reward += increment;
    m_Info.m_CumulativeReward += increment;
}
float GRAVEngine::AI::agent::cumulativeReward()
{
    return m_Info.m_CumulativeReward;
}

const float GRAVEngine::AI::agent::maxStep() const
{
    return m_AgentParams.m_MaxStepCount;
}

void GRAVEngine::AI::agent::onEpisodeBegin()
{
    m_AgentParams.m_EpisodeCount++;
}
void GRAVEngine::AI::agent::onEpisodeEnd()
{
}
GRAVEngine::uint32 GRAVEngine::AI::agent::completedEpisdes()
{
    return m_AgentParams.m_EpisodeCount;
}
void GRAVEngine::AI::agent::endEpisode()
{
    // End the episode because done was called
    endEpisodeAndReset(doneReason::DONE_CALLED);
}
void GRAVEngine::AI::agent::interuptEpisode()
{
    // End the episode, but not because of the agent's fault
    endEpisodeAndReset(doneReason::MAX_STEP_REACHED);
}
void GRAVEngine::AI::agent::endEpisodeAndReset(doneReason reason)
{
    markDone(reason);
    reset();
}

void GRAVEngine::AI::agent::reset()
{
    // Reset the data and set the step count to 0
    resetData();
    m_AgentParams.m_StepCount = 0;

    // Potential for recursion right here. Maybe check for it
    onEpisodeBegin();
}
void GRAVEngine::AI::agent::resetData()
{
    // Reset the actuators
    m_Actuators.reset();
}

//void GRAVEngine::AI::agent::onActionRecieved(Actions::actionBuffer buffers)
//{
//}



void GRAVEngine::AI::agent::safeInitialization()
{
    GRAV_PROFILE_FUNCTION();

    // Do nothing if already initialized
    if (m_Initialized)
        return;

    m_Initialized = true;

    // Get the atomic episodeID Counter
    m_Info.m_EpisodeID = episodeCounter::episodeId();
     
    // Add this agent's methods to the academy
    //trainer::instance()->m_AgentIncrementStepEvent.registerCallback(std::bind(&agent::incrementStep, this));
    environmentManager::instance().m_AgentIncrementStepEvent.registerCallback(GRAV_BIND_EVENT_FN(agent::incrementStep));
    environmentManager::instance().m_AgentSendInfoEvent.registerCallback(GRAV_BIND_EVENT_FN(agent::sendInfo));
    environmentManager::instance().m_DecideActionEvent.registerCallback(GRAV_BIND_EVENT_FN(agent::decideAction));

    environmentManager::instance().m_AgentActEvent.registerCallback(GRAV_BIND_EVENT_FN(agent::step));
    environmentManager::instance().m_AgentResetEvent.registerCallback(GRAV_BIND_EVENT_FN(agent::reset));

    // Ready the actuators for execution
    m_Actuators.readyForExecution();

    // Create the program for the agent based on the behavior parameters and the given sensors and actuators
    m_Brain = m_BehaviorParams.createProgram(m_Sensors, m_Actuators);

    // Preemptive data reset to guarantee the starting state
    resetData();

    // Call the custom initializer
    initialize();


    // Create the references for the action buffer
    auto continuous = createRef<Actions::actionBufferSegment<float>>(
        createScope<float[]>(m_Actuators.continuousActionCount()), m_Actuators.continuousActionCount());
    auto discrete = createRef<Actions::actionBufferSegment<int32>>(
        createScope<int32[]>(m_Actuators.discreteActionCount()), m_Actuators.discreteActionCount());
    m_Info.m_Actions = Actions::actionBuffer(continuous, discrete);

}
void GRAVEngine::AI::agent::initialize()
{
}
void GRAVEngine::AI::agent::deinitialize()
{
    GRAV_PROFILE_FUNCTION();
   
    // Remove the callbacks from the trainer
    if (GRAVEngine::AI::environmentManager::instance().initialized())
    {
        environmentManager::instance().m_AgentIncrementStepEvent.unregisterCallback(m_StepHandle);
        environmentManager::instance().m_AgentSendInfoEvent.unregisterCallback(m_SendInfoHandle);
        environmentManager::instance().m_DecideActionEvent.unregisterCallback(m_DecideActionHandle);

        environmentManager::instance().m_AgentActEvent.unregisterCallback(m_ActHandle);
        environmentManager::instance().m_AgentResetEvent.unregisterCallback(m_ResetHandle);
    }

    // Delete the agent program
    m_Brain = nullptr;

    m_Initialized = false;
}

void GRAVEngine::AI::agent::updateSensors()
{
    GRAV_PROFILE_FUNCTION();
  
    for (auto it = m_Sensors.begin(); it != m_Sensors.end(); it++)
    {
        (*it)->update();
    }
}
void GRAVEngine::AI::agent::resetSensors()
{
    GRAV_PROFILE_FUNCTION();
   
    for (auto it = m_Sensors.begin(); it != m_Sensors.end(); it++)
    {
        (*it)->reset();
    }
}
//void GRAVEngine::AI::agent::collectObservations(Sensors::vectorSensor& sensor)
//{
//}

void GRAVEngine::AI::agent::sendInfo()
{
    if (m_ShouldRequestDecision)
    {
        sendInfoToBrain();
        m_Info.m_Reward = 0;
        m_ShouldRequestDecision = false;
    }
}
void GRAVEngine::AI::agent::incrementStep()
{
    m_AgentParams.m_StepCount++;
}
void GRAVEngine::AI::agent::step()
{
    GRAV_PROFILE_FUNCTION();
   
    if (m_ShouldRequestAction && m_Brain != nullptr)
    {
        m_ShouldRequestAction = false;
        m_Actuators.executeActions();
    }

    if (m_AgentParams.m_StepCount >= m_AgentParams.m_MaxStepCount && m_AgentParams.m_MaxStepCount > 0)
    {
        markDone(doneReason::MAX_STEP_REACHED);
        reset();
    }
}
void GRAVEngine::AI::agent::decideAction()
{
    GRAV_PROFILE_FUNCTION();
    
    // Get the actions from the brain or use a default action buffer
    Actions::actionBuffer actions = m_Brain == nullptr ? Actions::actionBuffer() : m_Brain->decideAction();

    // Copy the created actions into the buffer, and pass the new information to the actuators
    m_Info.copyActions(actions);
    m_Actuators.updateActions(actions);
}
