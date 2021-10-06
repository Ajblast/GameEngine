#include "gravpch.h"
#include "actuatorList.h"
#include <algorithm>
#include <functional>

const GRAVEngine::AI::Actions::actionSpec GRAVEngine::AI::Actions::actuatorList::totalActionSpec()
{
	readyForExecution();
	return m_ActionSpec; 
}

GRAVEngine::AI::Actions::actuatorList::actuatorList() : actuatorList(0)
{
}

GRAVEngine::AI::Actions::actuatorList::actuatorList(size_t capacity) : m_ContinuousCount(0), m_DiscreteCount(0), m_TotalDiscreteActionsCount(0), m_Executable(false)
{
	m_Actuators.reserve(capacity);
}

void GRAVEngine::AI::Actions::actuatorList::readyForExecution()
{
	readyForExecution(continuousActionCount(), discreteActionCount(), totalDiscreteActionCount());

}

void GRAVEngine::AI::Actions::actuatorList::updateActions(actionBuffer actions)
{
	GRAV_PROFILE_FUNCTION();

	// Ready the action array and update the buffer
	readyForExecution();
	updateActionArray<float>(*actions.continuousActions(), actions.continuousActions()->size(), *m_ActionBuffer.continuousActions(), m_ActionBuffer.continuousActions()->size());
	updateActionArray<int32>(*actions.discreteActions(), actions.discreteActions()->size(), *m_ActionBuffer.discreteActions(), m_ActionBuffer.discreteActions()->size());
}
void GRAVEngine::AI::Actions::actuatorList::applyHeuristic(actionBuffer& actions)
{
	GRAV_PROFILE_FUNCTION();

	size_t continuousOffset = 0;
	size_t discreteOffset = 0;

	for (auto it = m_Actuators.begin(); it != m_Actuators.end(); it++)
	{
		size_t continuousCount = (*it)->getActionSpec().continuousActionCount();
		size_t discreteCount = (*it)->getActionSpec().discreteActionCount();

		// Do nothing if there are not actions to perform
		if (continuousCount == 0 && discreteCount == 0)
			continue;


		// Create segments that look into the buffers
		ref<actionBufferSegment<float>> continuousActions = createRef<actionBufferSegment<float>>(actions.continuousActions()->getArray(), continuousCount, 0);
		ref<actionBufferSegment<int32>> discreteActions = createRef<actionBufferSegment<int32>>(actions.discreteActions()->getArray(), discreteCount, 0);

		// Create the new buffer for the heuristic that is made of only the new small segments
		actionBuffer buffer = actionBuffer(continuousActions, discreteActions);

		// Apply the heuristic
		(*it)->heuristic(buffer);

		// Increase the offset into the large buffer
		continuousOffset += continuousCount;
		discreteOffset += discreteCount;
	}
}

void GRAVEngine::AI::Actions::actuatorList::executeActions()
{
	GRAV_PROFILE_FUNCTION();

	size_t continuousOffset = 0;
	size_t discreteOffset = 0;

	for (auto it = m_Actuators.begin(); it != m_Actuators.end(); it++)
	{
		size_t continuousCount = (*it)->getActionSpec().continuousActionCount();
		size_t discreteCount = (*it)->getActionSpec().discreteActionCount();

		// Do nothing if there are not actions to perform
		if (continuousCount == 0 && discreteCount == 0)
			continue;


		// Create segments that look into the buffers
		ref<actionBufferSegment<float>> continuousActions = createRef<actionBufferSegment<float>>(m_ActionBuffer.continuousActions()->getArray(), continuousCount, 0);
		ref<actionBufferSegment<int32>> discreteActions = createRef<actionBufferSegment<int32>>(m_ActionBuffer.discreteActions()->getArray(), discreteCount, 0);

		// Create the new buffer for the heuristic that is made of only the new small segments
		actionBuffer buffer = actionBuffer(continuousActions, discreteActions);

		// Execute the actions
		(*it)->onActionRecieved(buffer);

		// Increase the offset into the large buffer
		continuousOffset += continuousCount;
		discreteOffset += discreteCount;
	}
}
void GRAVEngine::AI::Actions::actuatorList::reset()
{
	if (m_Executable == false)
		return;

	// Clear the buffer
	m_ActionBuffer.clear();

	// Reset each actuator
	for (auto it = m_Actuators.begin(); it != m_Actuators.end(); it++)
		(*it)->reset();
}
void GRAVEngine::AI::Actions::actuatorList::validateActuators()
{
	// Compare each
	for (size_t i = 0; i < m_Actuators.size() - 1; i++)
	{
		GRAV_ASSERT(m_Actuators[i]->getName().compare(m_Actuators[i + 1]->getName()) != 0);
	}
}

void GRAVEngine::AI::Actions::actuatorList::addActuator(ref<IActuator> actuator)
{
	GRAV_ASSERT(m_Executable == false);
	// Do nothing if the actuator is already in the list
	if (contains(actuator) == true)
		return;

	m_Actuators.push_back(actuator);
	addToBuffer(actuator);
}
void GRAVEngine::AI::Actions::actuatorList::removeActuator(ref<IActuator> actuator)
{
	GRAV_ASSERT(m_Executable == false);
	// Do nothing if the actuator is not in the list
	if (contains(actuator) == false)
		return;

	// Erase the value from the vector
	m_Actuators.erase(std::find(m_Actuators.begin(), m_Actuators.end(), actuator));
	subToBuffer(actuator);
}
bool GRAVEngine::AI::Actions::actuatorList::contains(ref<IActuator> actuator)
{
	return std::find(m_Actuators.begin(), m_Actuators.end(), actuator) != m_Actuators.end();
}
void GRAVEngine::AI::Actions::actuatorList::clear()
{
	GRAV_ASSERT(m_Executable == false);

	// Remove the actuators and remove the internal counter
	m_Actuators.clear();
	clearBuffer();
}

void GRAVEngine::AI::Actions::actuatorList::readyForExecution(size_t continuousCount, size_t discreteCount, size_t totalDiscreteCount)
{
	// Don't do anything if the list is already executable
	if (m_Executable)
		return;

	// Sort all of the names of the actuators
	sortActuators(m_Actuators);

	// Make sure actuators are unique
	validateActuators();

	// Create the action arrays
	m_ContinuousActions = createScope<float[]>(continuousCount);
	m_DiscreteActions = createScope<int32[]>(discreteCount);

	// Create the segments
	ref<actionBufferSegment<float>> continuousSegment = createRef<actionBufferSegment<float>>(std::move(m_ContinuousActions), continuousCount);
	ref<actionBufferSegment<int32>> discreteSegment = createRef<actionBufferSegment<int32>>(std::move(m_DiscreteActions), discreteCount);

	// Create the action buffer
	m_ActionBuffer = actionBuffer(continuousSegment, discreteSegment);
	// Get the total action spec
	m_ActionSpec = getTotalActionSpec();

	// List should now be executable
	m_Executable = true;
}

GRAVEngine::AI::Actions::actionSpec GRAVEngine::AI::Actions::actuatorList::getTotalActionSpec()
{
	uint32 continuousCount = 0;
	uint32 discreteCount = 0;
	uint32 combinedBranchCount = 0;

	// Add all the actions together
	for (auto it = m_Actuators.begin(); it != m_Actuators.end(); it++)
	{
		continuousCount += (*it)->getActionSpec().continuousActionCount();
		discreteCount += (*it)->getActionSpec().discreteActionCount();
	}

	// All of the total branches
	scope<size_t[]> combinedBranches = createScope<size_t[]>(discreteCount);

	// Combine all of the branches together if needed
	if(discreteCount > 0)
	{
		// Offset into the array
		size_t arrayOffset = 0;

		// For each actuator
		for (auto it = m_Actuators.begin(); it != m_Actuators.end(); it++)
		{
			actionSpec spec = (*it)->getActionSpec();
			// Get the branch sizes and the branch count
			scope<size_t[]>& branchSizes = spec.branchSizes();
			size_t branchCount = spec.discreteActionCount();

			// Copy the branch sizes into the combined array
			memcpy(combinedBranches.get() + arrayOffset, branchSizes.get(), branchCount * sizeof(size_t));
			arrayOffset += branchCount;
		}
	}

	return actionSpec(continuousCount, discreteCount, std::move(combinedBranches));
}

void GRAVEngine::AI::Actions::actuatorList::sortActuators(std::vector<ref<IActuator>>)
{
	// Sort the actuators by name
	std::sort(m_Actuators.begin(), m_Actuators.end(), [](const ref<IActuator> lhs, const ref<IActuator> rhs) {return lhs->getName().compare(rhs->getName()); });
}

void GRAVEngine::AI::Actions::actuatorList::addToBuffer(ref<IActuator> actuator)
{
	m_ContinuousCount += actuator->getActionSpec().continuousActionCount();
	m_DiscreteCount += actuator->getActionSpec().discreteActionCount();
	m_TotalDiscreteActionsCount += actuator->getActionSpec().totalDiscreteActionCount();
}
void GRAVEngine::AI::Actions::actuatorList::subToBuffer(ref<IActuator> actuator)
{
	m_ContinuousCount -= actuator->getActionSpec().continuousActionCount();
	m_DiscreteCount -= actuator->getActionSpec().discreteActionCount();
	m_TotalDiscreteActionsCount -= actuator->getActionSpec().totalDiscreteActionCount();
}
void GRAVEngine::AI::Actions::actuatorList::clearBuffer()
{
	m_ContinuousCount = m_DiscreteCount = m_TotalDiscreteActionsCount = 0;
}
