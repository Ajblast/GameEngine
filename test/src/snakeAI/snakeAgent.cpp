#include "pch.h"
#include "snakeAgent.h"
#include <utils/randomInt.h>

snakeAgent::snakeAgent(int wallWidth, int wallHeight, GRAVEngine::AI::programParams params, size_t maxStep) : 
	agent(params, maxStep), m_MovementDirection(0), m_CurrentHeadPosition({ 0, 0 }), m_FruitPosition({0, 0}), m_WallWidth(wallWidth), m_WallHeight(wallHeight)
{
	m_Body.push_back(m_CurrentHeadPosition);
}
snakeAgent::snakeAgent(int wallWidth, int wallHeight, std::vector<GRAVEngine::ref<GRAVEngine::AI::Sensors::ISensor>>& sensors, std::vector<GRAVEngine::ref<GRAVEngine::AI::Actions::IActuator>>& actuators, GRAVEngine::AI::programParams programParams, size_t maxStep) :
	agent(sensors, actuators, programParams, maxStep), m_MovementDirection(0), m_CurrentHeadPosition({ 0, 0 }), m_FruitPosition({ 0, 0 }), m_WallWidth(wallWidth), m_WallHeight(wallHeight)
{
	m_Body.push_back(m_CurrentHeadPosition);
}

void snakeAgent::initialize()
{
	agent::initialize();
}

void snakeAgent::onEpisodeBegin()
{
	agent::onEpisodeBegin();

	// Reset the environment

	m_MovementDirection = GRAVEngine::randomInt(0, 3)[0];

	// Clear the body
	m_Body.clear();

	// Place the head and body
	//m_CurrentHeadPosition = { GRAVEngine::randomInt(0, m_WallWidth - 1)[0], GRAVEngine::randomInt(0, m_WallHeight - 1)[0] };
	m_CurrentHeadPosition = { m_WallWidth / 2, m_WallHeight / 2};
	m_OldHeadPosition = m_CurrentHeadPosition;
	//m_OldOldHeadPosition = m_CurrentHeadPosition;

	// Add the head to the body
	m_Body.push_back(m_CurrentHeadPosition);

	auto randX = GRAVEngine::randomInt(0, m_WallWidth - 1);
	auto randY = GRAVEngine::randomInt(0, m_WallHeight - 1);

	// "Randomly" place the fruit
	m_FruitPosition = { randX[0], randY[0] };

	// Reset lifespan
	currentLifespan = 0;

	bodySize = 1;
}

void snakeAgent::onEpisodeEnd()
{
	agent::onEpisodeEnd();
}
