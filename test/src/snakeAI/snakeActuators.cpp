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

	agent->m_MovementDirection = (int) buffers.discreteActions()->operator[](0);

	// Change the direction of the snake and move in that direction
	if (agent->m_MovementDirection == 0)
		agent->m_CurrentHeadPosition.m_Y++;
	else if (agent->m_MovementDirection == 1)
		agent->m_CurrentHeadPosition.m_X++;
	else if (agent->m_MovementDirection == 2)
		agent->m_CurrentHeadPosition.m_Y--;
	else if (agent->m_MovementDirection == 3)
		agent->m_CurrentHeadPosition.m_X--;


	//if (agent->m_MovementDirection == 0)
	//	agent->setReward(1);
	//else
	//	agent->setReward(-1);

	
	// Encourage getting to a fruit quickly by giving a penalty each step
	agent->addReward(-1.0f / agent->maxStep());

	float oldDistance =
		(agent->m_OldHeadPosition.m_X - agent->m_FruitPosition.m_X) * (agent->m_OldHeadPosition.m_X - agent->m_FruitPosition.m_X) +
		(agent->m_OldHeadPosition.m_Y - agent->m_FruitPosition.m_Y) * (agent->m_OldHeadPosition.m_Y - agent->m_FruitPosition.m_Y);

	float currentDistance =
		(agent->m_CurrentHeadPosition.m_X - agent->m_FruitPosition.m_X) * (agent->m_CurrentHeadPosition.m_X - agent->m_FruitPosition.m_X) +
		(agent->m_CurrentHeadPosition.m_Y - agent->m_FruitPosition.m_Y) * (agent->m_CurrentHeadPosition.m_Y - agent->m_FruitPosition.m_Y);

	
	if (oldDistance < currentDistance)
		agent->addReward(1.0f / (agent->m_WallWidth + agent->m_WallHeight));



	// Add the head to the list
	agent->m_Body.insert(agent->m_Body.begin(), agent->m_CurrentHeadPosition);
	// The head position changed
	agent->m_OldHeadPosition = agent->m_CurrentHeadPosition;

	// Check if a fruit was found
	if (agent->m_CurrentHeadPosition.m_X == agent->m_FruitPosition.m_X && agent->m_CurrentHeadPosition.m_Y == agent->m_FruitPosition.m_Y)
	{
		// The agent ate the fruit
		agent->setReward(1);
		agent->fruitsCaptured++;
		agent->bodySize++;

		// Randomly place the fruit
		auto randX = GRAVEngine::randomInt(0, agent->m_WallWidth - 1);
		auto randY = GRAVEngine::randomInt(0, agent->m_WallHeight - 1);

		agent->m_FruitPosition = { randX[0], randY[0] };
		agent->currentLifespan = 0;

	}
	else
		// The agent did not eat the fruit, so remove the "tail"
		agent->m_Body.pop_back();

	agent->currentLifespan++;



	// Check if the snake killed itself on the walls
	if (agent->m_CurrentHeadPosition.m_X < 0 || agent->m_CurrentHeadPosition.m_X > agent->m_WallWidth - 1)
	{
		agent->wallsHit++;
		agent->setReward(-1);
		agent->endEpisode();
		return;
	}
	if (agent->m_CurrentHeadPosition.m_Y < 0 || agent->m_CurrentHeadPosition.m_Y > agent->m_WallHeight - 1)
	{
		agent->wallsHit++;
		agent->setReward(-1);
		agent->endEpisode();
		return;
	}

	// Check if the snake killed itself
	for (auto it = agent->m_Body.begin(); it != agent->m_Body.end(); it++)
	{
		if (it == agent->m_Body.begin())
			continue;

		if (agent->m_CurrentHeadPosition.m_X == it->m_X && agent->m_CurrentHeadPosition.m_Y == it->m_Y)
		{
			agent->bodyHit++;
			agent->setReward(-1);
			agent->endEpisode();
			return;
		}
	}
}

void directionActuator::heuristic(GRAVEngine::AI::Actions::actionBuffer& buffers)
{
}
