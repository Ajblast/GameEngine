#pragma once

#include "GRAVEngine.h"
#include <vector>

struct bodyPosition
{
public:
	int m_X, m_Y;
};

class snakeAgent : public GRAVEngine::AI::agent
{
public:
	snakeAgent(int wallWidth, int wallHeight, GRAVEngine::AI::programParams params);
	snakeAgent(int wallWidth, int wallHeight, std::vector<GRAVEngine::ref<GRAVEngine::AI::Sensors::ISensor>>& sensors, std::vector<GRAVEngine::ref<GRAVEngine::AI::Actions::IActuator>>& actuators, GRAVEngine::AI::programParams programParams);

	virtual ~snakeAgent() = default;

	// Initialze the agent. Meant for custom initialization. Sensors and actuators must have already been passed within the constructor.
	// Newly added sensors and actuators will not be allocated.
	virtual void initialize();

	// Called when the episode begins
	virtual void onEpisodeBegin() override;
	// Called when the episode ends
	virtual void onEpisodeEnd() override;

public:
	float m_MovementDirection;
	bodyPosition m_CurrentHeadPosition;		// The current head position;
	std::vector<bodyPosition> m_Body;		// The snake's body. The first in the list is the head and the last is the tail
	bodyPosition m_FruitPosition;

	int m_WallWidth = 20, m_WallHeight = 20;
};


// TODO::******************** implement the vector observation because I need a simple way 