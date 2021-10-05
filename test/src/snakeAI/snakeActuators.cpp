#include "pch.h"
#include "snakeActuators.h"
#include "utils/randomInt.h"

directionActuator::directionActuator(GRAVEngine::ref<snakeAgent> agent) : agent(agent)
{
	// Create the action spec
	GRAVEngine::scope<size_t[]> branchSizes = GRAVEngine::createScope<size_t[]>(1);
	branchSizes[0] = 4;
	m_ActionSpec = GRAVEngine::AI::Actions::actionSpec(0, 1, std::move(branchSizes));
}
std::string directionActuator::getName() const
{
	return "Direction Actuator";
}
void directionActuator::reset()
{
	// Does nothing
}
GRAVEngine::AI::Actions::actionSpec directionActuator::getActionSpec()
{
	return m_ActionSpec;
}

void directionActuator::onActionRecieved(GRAVEngine::AI::Actions::actionBuffer buffers)
{
	bool foodConsumed = false;

	// Check if the agent moved in the exact opposite direction and punish it harshly if it did and end the episode
	if (agent->m_MovementDirection == 0 && buffers.discreteActions()->operator[](0) == 2)
	{
		agent->timesMovingBackwards++;
		agent->setReward(-10);
		agent->endEpisode();
	}
	else if (agent->m_MovementDirection == 1 && buffers.discreteActions()->operator[](0) == 3)
	{
		agent->timesMovingBackwards++;
		agent->setReward(-10);
		agent->endEpisode();
	}
	else if (agent->m_MovementDirection == 2 && buffers.discreteActions()->operator[](0) == 0)
	{
		agent->timesMovingBackwards++;
		agent->setReward(-10);
		agent->endEpisode();
	}
	else if (agent->m_MovementDirection == 3 && buffers.discreteActions()->operator[](0) == 1)
	{
		agent->timesMovingBackwards++;
		agent->setReward(-10);
		agent->endEpisode();
	}

	agent->m_MovementDirection = buffers.discreteActions()->operator[](0);

	//GRAV_LOG_LINE_CRITICAL("Movement Direction %i", buffers.discreteActions()->operator[](0));

	// Change the direction of the snake and move in that direction
	if (agent->m_MovementDirection == 0)
		agent->m_CurrentHeadPosition.m_Y--;
	else if (agent->m_MovementDirection == 1)
		agent->m_CurrentHeadPosition.m_X++;
	else if (agent->m_MovementDirection == 2)
		agent->m_CurrentHeadPosition.m_Y++;
	else if (agent->m_MovementDirection == 3)
		agent->m_CurrentHeadPosition.m_X--;

	// Add the head to the list
	agent->m_Body.insert(agent->m_Body.begin(), agent->m_CurrentHeadPosition);


	// Check if a fruit was found
	if (agent->m_CurrentHeadPosition.m_X == agent->m_FruitPosition.m_X && agent->m_CurrentHeadPosition.m_Y == agent->m_FruitPosition.m_Y)
	{
		// The agent ate the fruit
		agent->addReward(5);
		agent->fruitsCaptured++;
		foodConsumed = true;
	}
	else
		// The agent did not eat the fruit, so remove the "tail"
		agent->m_Body.pop_back();

	// Randomly add the fruit to the screen
	if (foodConsumed)
	{
		auto randX = GRAVEngine::randomInt(0, agent->m_WallWidth - 1);
		auto randY = GRAVEngine::randomInt(0, agent->m_WallHeight - 1);

		agent->m_FruitPosition = { randX[0], randY[0] };
	}

	// Check if the snake killed itself on the walls
	if (agent->m_CurrentHeadPosition.m_X < 0 || agent->m_CurrentHeadPosition.m_X > agent->m_WallWidth - 1)
	{
		agent->wallsHit++;
		agent->addReward(-1);
		agent->endEpisode();
	}
	if (agent->m_CurrentHeadPosition.m_Y < 0 || agent->m_CurrentHeadPosition.m_Y > agent->m_WallHeight - 1)
	{
		agent->wallsHit++;
		agent->addReward(-1);
		agent->endEpisode();
	}

	// Check if the snake killed itself
	for (auto it = agent->m_Body.begin() + 1; it != agent->m_Body.end(); it++)
	{
		if (agent->m_CurrentHeadPosition.m_X == it->m_X && agent->m_CurrentHeadPosition.m_Y == it->m_Y)
		{
			agent->bodyHit++;
			agent->addReward(-1);
			agent->endEpisode();
		}
	}
}

void directionActuator::heuristic(GRAVEngine::AI::Actions::actionBuffer& buffers)
{
}
