#include "pch.h"
#include "snakeSensors.h"

directionSensor::directionSensor(GRAVEngine::ref<snakeAgent> agent) : agent(agent)
{
}
size_t directionSensor::write(GRAVEngine::AI::Sensors::observationWriter& writer)
{
	writer.write(direction, 1);
	return 1;
}
void directionSensor::update()
{
	// Does nothing
	direction[0] = agent->m_MovementDirection;
}
void directionSensor::reset()
{
	// Does nothing
	direction[0] = 0;
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
	writer.write(position, 4);

	return 4;
}
void headSensor::update()
{
	position[0] = agent->m_CurrentHeadPosition.m_X;
	position[1] = agent->m_CurrentHeadPosition.m_Y;
	position[2] = agent->m_OldHeadPosition.m_X;
	position[3] = agent->m_OldHeadPosition.m_Y;
}
void headSensor::reset()
{
	position[0] = 0;
	position[1] = 0;
	position[2] = 0;
	position[3] = 0;
}
GRAVEngine::AI::Sensors::observationParams headSensor::getObservationParams()
{
	return GRAVEngine::AI::Sensors::observationParams(4);
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
	//writer.write(relativePosition, 6);
	//return 6;
	writer.write(relativePosition, 4);
	return 4;
}
void fruitSensor::update()
{
	relativePosition[0] = agent->m_CurrentHeadPosition.m_X - agent->m_FruitPosition.m_X;
	relativePosition[1] = agent->m_CurrentHeadPosition.m_Y - agent->m_FruitPosition.m_Y;
	relativePosition[2] = agent->m_OldHeadPosition.m_X - agent->m_FruitPosition.m_X;
	relativePosition[3] = agent->m_OldHeadPosition.m_Y - agent->m_FruitPosition.m_Y;
	//relativePosition[4] = agent->m_FruitPosition.m_X;
	//relativePosition[5] = agent->m_FruitPosition.m_Y;
}
void fruitSensor::reset()
{
	relativePosition[0] = 0;
	relativePosition[1] = 0;
	relativePosition[2] = 0;
	relativePosition[3] = 0;
	//relativePosition[4] = 0;
	//relativePosition[5] = 0;
}
GRAVEngine::AI::Sensors::observationParams fruitSensor::getObservationParams()
{
	return GRAVEngine::AI::Sensors::observationParams(4);
	//return GRAVEngine::AI::Sensors::observationParams(6);
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
	writer.write(relativePositions, 4);
	return 4;
}
void wallSensor::update()
{
	// Have to add 1 to the relative distance because the valid playing area is from 0-19, but don't need to add when comparing body parts
	relativePositions[0] = std::abs(m_WallHeight - agent->m_CurrentHeadPosition.m_Y);	// Up
	relativePositions[1] = std::abs(m_WallWidth - agent->m_CurrentHeadPosition.m_X);	// Right
	relativePositions[2] = std::abs(agent->m_CurrentHeadPosition.m_Y + 1);				// Down
	relativePositions[3] = std::abs(agent->m_CurrentHeadPosition.m_X + 1);				// Left

	if (m_AccountBody)
		for (auto it = agent->m_Body.begin(); it != agent->m_Body.end(); it++)
		{
			// Up Most
			if (it->m_X == agent->m_CurrentHeadPosition.m_X && it->m_Y > agent->m_CurrentHeadPosition.m_Y && std::abs(agent->m_CurrentHeadPosition.m_Y - it->m_Y) < relativePositions[0])
				relativePositions[0] = std::abs(agent->m_CurrentHeadPosition.m_Y - it->m_Y);
			// Right Most
			if (it->m_Y == agent->m_CurrentHeadPosition.m_Y && it->m_X > agent->m_CurrentHeadPosition.m_X && std::abs(agent->m_CurrentHeadPosition.m_X - it->m_X) < relativePositions[1])
				relativePositions[1] = std::abs(agent->m_CurrentHeadPosition.m_X - it->m_X);
			// Down Most
			if (it->m_X == agent->m_CurrentHeadPosition.m_X && it->m_Y < agent->m_CurrentHeadPosition.m_Y && std::abs(agent->m_CurrentHeadPosition.m_Y - it->m_Y) < relativePositions[2])
				relativePositions[2] = std::abs(agent->m_CurrentHeadPosition.m_Y - it->m_Y);
			// Left
			if (it->m_Y == agent->m_CurrentHeadPosition.m_Y && it->m_X < agent->m_CurrentHeadPosition.m_X && std::abs(agent->m_CurrentHeadPosition.m_X - it->m_X) < relativePositions[3])
				relativePositions[3] = std::abs(agent->m_CurrentHeadPosition.m_X - it->m_X);
		}

}
void wallSensor::reset()
{
	relativePositions[0] = 0;
	relativePositions[1] = 0;
	relativePositions[2] = 0;
	relativePositions[3] = 0;
}
GRAVEngine::AI::Sensors::observationParams wallSensor::getObservationParams()
{
	return GRAVEngine::AI::Sensors::observationParams(4);
}
std::string wallSensor::getName()
{
	return "Wall Sensor";
}

bodySizeSensor::bodySizeSensor(GRAVEngine::ref<snakeAgent> agent) : agent(agent)
{
}
size_t bodySizeSensor::write(GRAVEngine::AI::Sensors::observationWriter& writer)
{
	writer.write(bodySize, 1);
	return 1;
}
void bodySizeSensor::update()
{
	bodySize[0] = agent->bodySize;
}
void bodySizeSensor::reset()
{
	bodySize[0] = 1;
}
GRAVEngine::AI::Sensors::observationParams bodySizeSensor::getObservationParams()
{
	return GRAVEngine::AI::Sensors::observationParams(1);
}
std::string bodySizeSensor::getName()
{
	return "Body Size Sensor";
}


//wallSensor::wallSensor(GRAVEngine::ref<snakeAgent> agent, int wallWidth, int wallHeight, bool accountBody) : agent(agent), m_WallWidth(wallWidth), m_WallHeight(wallHeight), m_AccountBody(accountBody)
//{
//}
//size_t wallSensor::write(GRAVEngine::AI::Sensors::observationWriter& writer)
//{
//	writer.write(relativePositions, 3);
//	return 3;
//}
//void wallSensor::update()
//{
//	if (agent->m_MovementDirection == 0)
//	{
//		// Agent moving up
//
//		relativePositions[0] = agent->m_CurrentHeadPosition.m_X;
//		relativePositions[1] = agent->m_CurrentHeadPosition.m_Y;
//		relativePositions[2] = m_WallWidth - agent->m_CurrentHeadPosition.m_X;
//
//		if (m_AccountBody)
//			for (auto it = agent->m_Body.begin(); it != agent->m_Body.end(); it++)
//			{
//				// Left most
//				if (it->m_Y == agent->m_CurrentHeadPosition.m_Y && it->m_X < agent->m_CurrentHeadPosition.m_X && std::abs(agent->m_CurrentHeadPosition.m_X - it->m_X) < relativePositions[0])
//					relativePositions[0] = std::abs(agent->m_CurrentHeadPosition.m_X - it->m_X);
//				// Forward most
//				if (it->m_X == agent->m_CurrentHeadPosition.m_X && it->m_Y < agent->m_CurrentHeadPosition.m_Y && std::abs(agent->m_CurrentHeadPosition.m_Y - it->m_Y) < relativePositions[1])
//					relativePositions[1] = std::abs(agent->m_CurrentHeadPosition.m_Y - it->m_Y);
//				// Right
//				if (it->m_Y == agent->m_CurrentHeadPosition.m_Y && it->m_X > agent->m_CurrentHeadPosition.m_X && std::abs(agent->m_CurrentHeadPosition.m_X - it->m_X) < relativePositions[2])
//					relativePositions[2] = std::abs(agent->m_CurrentHeadPosition.m_X - it->m_X);
//			}
//	}
//	else if (agent->m_MovementDirection == 1)
//	{
//		// Agent moving right
//
//		relativePositions[0] = agent->m_CurrentHeadPosition.m_Y;
//		relativePositions[1] = m_WallWidth - agent->m_CurrentHeadPosition.m_X;
//		relativePositions[2] = m_WallHeight - agent->m_CurrentHeadPosition.m_Y;
//
//		if (m_AccountBody)
//			for (auto it = agent->m_Body.begin(); it != agent->m_Body.end(); it++)
//			{
//				// Left most
//				if (it->m_X == agent->m_CurrentHeadPosition.m_X && it->m_Y < agent->m_CurrentHeadPosition.m_Y && std::abs(agent->m_CurrentHeadPosition.m_Y - it->m_Y) < relativePositions[0])
//					relativePositions[0] = std::abs(agent->m_CurrentHeadPosition.m_Y - it->m_Y);
//				// Forward most
//				if (it->m_Y == agent->m_CurrentHeadPosition.m_Y && it->m_X > agent->m_CurrentHeadPosition.m_X && std::abs(agent->m_CurrentHeadPosition.m_X - it->m_X) < relativePositions[1])
//					relativePositions[1] = std::abs(agent->m_CurrentHeadPosition.m_X - it->m_X);
//				// Right
//				if (it->m_X == agent->m_CurrentHeadPosition.m_X && it->m_Y > agent->m_CurrentHeadPosition.m_Y && std::abs(agent->m_CurrentHeadPosition.m_Y - it->m_Y) < relativePositions[2])
//					relativePositions[2] = std::abs(agent->m_CurrentHeadPosition.m_Y - it->m_Y);
//			}
//
//	}
//	else if (agent->m_MovementDirection == 2)
//	{
//		// Agent moving down
//
//		relativePositions[0] = m_WallWidth - agent->m_CurrentHeadPosition.m_X;
//		relativePositions[1] = m_WallHeight - agent->m_CurrentHeadPosition.m_Y;
//		relativePositions[2] = agent->m_CurrentHeadPosition.m_X;
//
//		if (m_AccountBody)
//			for (auto it = agent->m_Body.begin(); it != agent->m_Body.end(); it++)
//			{
//				// Left most
//				if (it->m_Y == agent->m_CurrentHeadPosition.m_Y && it->m_X > agent->m_CurrentHeadPosition.m_X && std::abs(agent->m_CurrentHeadPosition.m_X - it->m_X) < relativePositions[0])
//					relativePositions[0] = std::abs(agent->m_CurrentHeadPosition.m_X - it->m_X);
//				// Forward most
//				if (it->m_X == agent->m_CurrentHeadPosition.m_X && it->m_Y > agent->m_CurrentHeadPosition.m_Y && std::abs(agent->m_CurrentHeadPosition.m_Y - it->m_Y) < relativePositions[1])
//					relativePositions[1] = std::abs(agent->m_CurrentHeadPosition.m_Y - it->m_Y);
//				// Right
//				if (it->m_Y == agent->m_CurrentHeadPosition.m_Y && it->m_X < agent->m_CurrentHeadPosition.m_X && std::abs(agent->m_CurrentHeadPosition.m_X - it->m_X) < relativePositions[2])
//					relativePositions[2] = std::abs(agent->m_CurrentHeadPosition.m_X - it->m_X);
//			}
//	}
//	else
//	{
//		// Agent moving left
//
//		relativePositions[0] = m_WallHeight - agent->m_CurrentHeadPosition.m_Y;
//		relativePositions[1] = agent->m_CurrentHeadPosition.m_X;
//		relativePositions[2] = agent->m_CurrentHeadPosition.m_Y;
//
//		if (m_AccountBody)
//			for (auto it = agent->m_Body.begin(); it != agent->m_Body.end(); it++)
//			{
//				// Left most
//				if (it->m_X == agent->m_CurrentHeadPosition.m_X && it->m_Y > agent->m_CurrentHeadPosition.m_Y && std::abs(agent->m_CurrentHeadPosition.m_Y - it->m_Y) < relativePositions[0])
//					relativePositions[0] = std::abs(agent->m_CurrentHeadPosition.m_Y - it->m_Y);
//				// Forward most
//				if (it->m_Y == agent->m_CurrentHeadPosition.m_Y && it->m_X < agent->m_CurrentHeadPosition.m_X && std::abs(agent->m_CurrentHeadPosition.m_X - it->m_X) < relativePositions[1])
//					relativePositions[1] = std::abs(agent->m_CurrentHeadPosition.m_X - it->m_X);
//				// Right
//				if (it->m_X == agent->m_CurrentHeadPosition.m_X && it->m_Y < agent->m_CurrentHeadPosition.m_Y && std::abs(agent->m_CurrentHeadPosition.m_Y - it->m_Y) < relativePositions[2])
//					relativePositions[2] = std::abs(agent->m_CurrentHeadPosition.m_Y - it->m_Y);
//			}
//	}
//}
//void wallSensor::reset()
//{
//	relativePositions[0] = 0;
//	relativePositions[1] = 0;
//	relativePositions[2] = 0;
//}
//GRAVEngine::AI::Sensors::observationParams wallSensor::getObservationParams()
//{
//	return GRAVEngine::AI::Sensors::observationParams(3);
//}
//std::string wallSensor::getName()
//{
//	return "Wall Sensor";
//}