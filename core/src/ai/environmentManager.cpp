#include "gravpch.h"
#include "environmentManager.h"

// The static trainer instance
GRAVEngine::AI::environmentManager& GRAVEngine::AI::environmentManager::s_Instance = GRAVEngine::AI::environmentManager();


void GRAVEngine::AI::environmentManager::step()
{
	Locks::scopedLock<decltype(m_TrainerLock)> lock(m_TrainerLock);
	
	GRAV_PROFILE_FUNCTION();

	// Can only step if there is an instance
	if (instance().initialized() == false)
		return;

	//GRAV_LOG_LINE_DEBUG("%s: Step environmentManager", GRAV_CLEAN_FUNC_SIG);

	// Increase the steps
	m_StepCount++;
	m_TotalStepCount++;

	// Increment the agent Steps
	m_AgentIncrementStepEvent.execute();

	// Send all the agent infos
	m_AgentSendInfoEvent.execute();

	// Decide agent actions
	m_DecideActionEvent.execute();

	// Act on decided actions
	m_AgentActEvent.execute();

	// Tell the trainer controller to update all of the algorithms
	m_TrainerController->step();

}
void GRAVEngine::AI::environmentManager::reset()
{
	Locks::scopedLock<decltype(m_TrainerLock)> lock(m_TrainerLock);
	
	GRAV_PROFILE_FUNCTION();

	// Can only step if there is an instance
	if (instance().initialized() == false)
		return;

	GRAV_LOG_LINE_DEBUG("%s: Reset environmentManager", GRAV_CLEAN_FUNC_SIG);

	// Reset the environment
	m_OnEnvironmentReset.execute();
	// Reset the agents
	m_AgentResetEvent.execute();
}

GRAVEngine::AI::environmentManager::environmentManager() : m_Initialized(false), m_EpisodeCount(0), m_StepCount(0), m_TotalStepCount(0), m_TrainerController(nullptr)
{
}
GRAVEngine::AI::environmentManager::~environmentManager()
{
	// Guarantee that the trainer deinitializes 
	deinitialize();
}

void GRAVEngine::AI::environmentManager::initialize(scope<Training::algorithmFactory> algorithmFactory)
{
	Locks::scopedLock<decltype(m_TrainerLock)> lock(m_TrainerLock);
	GRAV_PROFILE_FUNCTION();
	

	// Only a single instance of the trainer is allowed
	if(instance().initialized())
		return;

	GRAV_LOG_LINE_DEBUG("%s: Initialize environmentManager", GRAV_CLEAN_FUNC_SIG);

	// Create the trainer controller during initialization
	m_TrainerController = createScope<Training::trainerController>(std::move(algorithmFactory));

	m_Initialized = true;

	// Set the instance
	//s_Instance = this;

}
void GRAVEngine::AI::environmentManager::deinitialize()
{
	Locks::scopedLock<decltype(m_TrainerLock)> lock(m_TrainerLock);
	GRAV_PROFILE_FUNCTION();
	

	if (instance().initialized() == false)
		return;

	GRAV_LOG_LINE_DEBUG("%s: Deinitializing environmentManager", GRAV_CLEAN_FUNC_SIG);

	// Remove all actions
	m_DecideActionEvent.execute();


	// Clear the events
	m_DecideActionEvent.clear();
	m_DestroyActionEvent.clear();
	m_AgentIncrementStepEvent.clear();

	m_AgentSendInfoEvent.clear();
	m_AgentResetEvent.clear();
	m_AgentActEvent.clear();

	m_OnEnvironmentReset.clear();

	// Null the static instance
	//s_Instance = nullptr;
	m_Initialized = false;
}

GRAVEngine::AI::Training::trainerController& GRAVEngine::AI::environmentManager::controller()
{
	if (initialized() == false)
		throw std::runtime_error("Attempting to get controller for an uninitialized environment manager");

	return *m_TrainerController.get();
}
