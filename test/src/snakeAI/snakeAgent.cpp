#include "snakeAgent.h"

snakeAgent::snakeAgent(int wallWidth, int wallHeight, GRAVEngine::AI::programParams params) : 
	agent(params), m_MovementDirection(0), m_CurrentHeadPosition({ 0, 0 }), m_FruitPosition({0, 0}), m_WallWidth(wallWidth), m_WallHeight(wallHeight)
{
	m_Body.push_back(m_CurrentHeadPosition);
}
snakeAgent::snakeAgent(int wallWidth, int wallHeight, std::vector<GRAVEngine::ref<GRAVEngine::AI::Sensors::ISensor>>& sensors, std::vector<GRAVEngine::ref<GRAVEngine::AI::Actions::IActuator>>& actuators, GRAVEngine::AI::programParams programParams) :
	agent(sensors, actuators, programParams), m_MovementDirection(0), m_CurrentHeadPosition({ 0, 0 }), m_FruitPosition({ 0, 0 }), m_WallWidth(wallWidth), m_WallHeight(wallHeight)
{
	m_Body.push_back(m_CurrentHeadPosition);
}

void snakeAgent::initialize()
{
}

void snakeAgent::onEpisodeBegin()
{
	// Reset the environment

	// Clear the body
	m_Body.clear();

	// Place the head and body
	m_CurrentHeadPosition = { m_WallWidth / 2, m_WallHeight / 2 };

	// Add the head to the body
	m_Body.push_back(m_CurrentHeadPosition);

	// "Randomly" place the fruit
	m_FruitPosition = { m_WallWidth / 3, m_WallHeight / 3 };
}

void snakeAgent::onEpisodeEnd()
{
}
