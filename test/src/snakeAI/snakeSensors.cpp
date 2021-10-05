#include "pch.h"
#include "snakeSensors.h"

directionSensor::directionSensor(GRAVEngine::ref<snakeAgent> agent) : agent(agent)
{
}
size_t directionSensor::write(GRAVEngine::AI::Sensors::observationWriter& writer)
{
	float data[1] = { agent->m_MovementDirection };
	writer.write(data, 1);
	return 1;
}
void directionSensor::update()
{
	// Does nothing
}
void directionSensor::reset()
{
	// Does nothing
}
GRAVEngine::AI::Sensors::observationParams directionSensor::getObservationParams()
{
	return GRAVEngine::AI::Sensors::observationParams(1);
}
std::string directionSensor::getName()
{
	return "Direction Sensor";
}



headSensor::headSensor(GRAVEngine::ref<snakeAgent> agent) : agent(agent)
{
}
size_t headSensor::write(GRAVEngine::AI::Sensors::observationWriter& writer)
{
	float data[2] = { agent->m_CurrentHeadPosition.m_X, agent->m_CurrentHeadPosition.m_Y };
	writer.write(data, 2);

	return 2;
}
void headSensor::update()
{
}
void headSensor::reset()
{
}
GRAVEngine::AI::Sensors::observationParams headSensor::getObservationParams()
{
	return GRAVEngine::AI::Sensors::observationParams(2);
}
std::string headSensor::getName()
{
	return "Head Sensor";
}


fruitSensor::fruitSensor(GRAVEngine::ref<snakeAgent> agent) : agent(agent)
{
}
size_t fruitSensor::write(GRAVEngine::AI::Sensors::observationWriter& writer)
{
	// Write the internal position
	writer.write(relativePosition, 2);
	return 2;
}
void fruitSensor::update()
{
	relativePosition[0] = agent->m_CurrentHeadPosition.m_X - agent->m_FruitPosition.m_X;
	relativePosition[1] = agent->m_CurrentHeadPosition.m_Y - agent->m_FruitPosition.m_Y;
}
void fruitSensor::reset()
{
	relativePosition[0] = 0;
	relativePosition[1] = 0;
}
GRAVEngine::AI::Sensors::observationParams fruitSensor::getObservationParams()
{
	return GRAVEngine::AI::Sensors::observationParams(2);
}
std::string fruitSensor::getName()
{
	return "Fruit Sensor";
}


wallSensor::wallSensor(GRAVEngine::ref<snakeAgent> agent, int wallWidth, int wallHeight, bool accountBody) : agent(agent), m_WallWidth(wallWidth), m_WallHeight(wallHeight), m_AccountBody(accountBody)
{
}
size_t wallSensor::write(GRAVEngine::AI::Sensors::observationWriter& writer)
{
	writer.write(relativePositions, 3);
	return 3;
}
void wallSensor::update()
{
	if (agent->m_MovementDirection == 0)
	{
		// Agent moving up

		relativePositions[0] = agent->m_CurrentHeadPosition.m_X;
		relativePositions[1] = agent->m_CurrentHeadPosition.m_Y;
		relativePositions[2] = m_WallWidth - agent->m_CurrentHeadPosition.m_X;

		if (m_AccountBody)
			for (auto it = agent->m_Body.begin(); it != agent->m_Body.end(); it++)
			{
				// Left most
				if (it->m_Y == agent->m_CurrentHeadPosition.m_Y && it->m_X < agent->m_CurrentHeadPosition.m_X && std::abs(agent->m_CurrentHeadPosition.m_X - it->m_X) < relativePositions[0])
					relativePositions[0] = std::abs(agent->m_CurrentHeadPosition.m_X - it->m_X);
				// Forward most
				if (it->m_X == agent->m_CurrentHeadPosition.m_X && it->m_Y < agent->m_CurrentHeadPosition.m_Y && std::abs(agent->m_CurrentHeadPosition.m_Y - it->m_Y) < relativePositions[1])
					relativePositions[1] = std::abs(agent->m_CurrentHeadPosition.m_Y - it->m_Y);
				// Right
				if (it->m_Y == agent->m_CurrentHeadPosition.m_Y && it->m_X > agent->m_CurrentHeadPosition.m_X && std::abs(agent->m_CurrentHeadPosition.m_X - it->m_X) < relativePositions[2])
					relativePositions[2] = std::abs(agent->m_CurrentHeadPosition.m_X - it->m_X);
			}
	}
	else if (agent->m_MovementDirection == 1)
	{
		// Agent moving right

		relativePositions[0] = agent->m_CurrentHeadPosition.m_Y;
		relativePositions[1] = m_WallWidth - agent->m_CurrentHeadPosition.m_X;
		relativePositions[2] = m_WallHeight - agent->m_CurrentHeadPosition.m_Y;

		if (m_AccountBody)
			for (auto it = agent->m_Body.begin(); it != agent->m_Body.end(); it++)
			{
				// Left most
				if (it->m_X == agent->m_CurrentHeadPosition.m_X && it->m_Y < agent->m_CurrentHeadPosition.m_Y && std::abs(agent->m_CurrentHeadPosition.m_Y - it->m_Y) < relativePositions[0])
					relativePositions[0] = std::abs(agent->m_CurrentHeadPosition.m_Y - it->m_Y);
				// Forward most
				if (it->m_Y == agent->m_CurrentHeadPosition.m_Y && it->m_X > agent->m_CurrentHeadPosition.m_X && std::abs(agent->m_CurrentHeadPosition.m_X - it->m_X) < relativePositions[1])
					relativePositions[1] = std::abs(agent->m_CurrentHeadPosition.m_X - it->m_X);
				// Right
				if (it->m_X == agent->m_CurrentHeadPosition.m_X && it->m_Y > agent->m_CurrentHeadPosition.m_Y && std::abs(agent->m_CurrentHeadPosition.m_Y - it->m_Y) < relativePositions[2])
					relativePositions[2] = std::abs(agent->m_CurrentHeadPosition.m_Y - it->m_Y);
			}

	}
	else if (agent->m_MovementDirection == 2)
	{
		// Agent moving down

		relativePositions[0] = m_WallWidth - agent->m_CurrentHeadPosition.m_X;
		relativePositions[1] = m_WallHeight - agent->m_CurrentHeadPosition.m_Y;
		relativePositions[2] = agent->m_CurrentHeadPosition.m_X;
	
		if (m_AccountBody)
			for (auto it = agent->m_Body.begin(); it != agent->m_Body.end(); it++)
			{
				// Left most
				if (it->m_Y == agent->m_CurrentHeadPosition.m_Y && it->m_X > agent->m_CurrentHeadPosition.m_X && std::abs(agent->m_CurrentHeadPosition.m_X - it->m_X) < relativePositions[0])
					relativePositions[0] = std::abs(agent->m_CurrentHeadPosition.m_X - it->m_X);
				// Forward most
				if (it->m_X == agent->m_CurrentHeadPosition.m_X && it->m_Y > agent->m_CurrentHeadPosition.m_Y && std::abs(agent->m_CurrentHeadPosition.m_Y - it->m_Y) < relativePositions[1])
					relativePositions[1] = std::abs(agent->m_CurrentHeadPosition.m_Y - it->m_Y);
				// Right
				if (it->m_Y == agent->m_CurrentHeadPosition.m_Y && it->m_X < agent->m_CurrentHeadPosition.m_X && std::abs(agent->m_CurrentHeadPosition.m_X - it->m_X) < relativePositions[2])
					relativePositions[2] = std::abs(agent->m_CurrentHeadPosition.m_X - it->m_X);
			}
	}
	else
	{
		// Agent moving left
	
		relativePositions[0] = m_WallHeight - agent->m_CurrentHeadPosition.m_Y;
		relativePositions[1] = agent->m_CurrentHeadPosition.m_X;
		relativePositions[2] = agent->m_CurrentHeadPosition.m_Y;
		
		if (m_AccountBody)
			for (auto it = agent->m_Body.begin(); it != agent->m_Body.end(); it++)
			{
				// Left most
				if (it->m_X == agent->m_CurrentHeadPosition.m_X && it->m_Y > agent->m_CurrentHeadPosition.m_Y && std::abs(agent->m_CurrentHeadPosition.m_Y - it->m_Y) < relativePositions[0])
					relativePositions[0] = std::abs(agent->m_CurrentHeadPosition.m_Y - it->m_Y);
				// Forward most
				if (it->m_Y == agent->m_CurrentHeadPosition.m_Y && it->m_X < agent->m_CurrentHeadPosition.m_X && std::abs(agent->m_CurrentHeadPosition.m_X - it->m_X) < relativePositions[1])
					relativePositions[1] = std::abs(agent->m_CurrentHeadPosition.m_X - it->m_X);
				// Right
				if (it->m_X == agent->m_CurrentHeadPosition.m_X && it->m_Y < agent->m_CurrentHeadPosition.m_Y && std::abs(agent->m_CurrentHeadPosition.m_Y - it->m_Y) < relativePositions[2])
					relativePositions[2] = std::abs(agent->m_CurrentHeadPosition.m_Y - it->m_Y);
			}
	}
}
void wallSensor::reset()
{
	relativePositions[0] = 0;
	relativePositions[1] = 0;
	relativePositions[2] = 0;
}
GRAVEngine::AI::Sensors::observationParams wallSensor::getObservationParams()
{
	return GRAVEngine::AI::Sensors::observationParams(3);
}
std::string wallSensor::getName()
{
	return "Wall Sensor";
}
