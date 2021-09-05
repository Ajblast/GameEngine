#pragma once

#include "GRAVEngine.h"
#include "snakeAgent.h"

class directionActuator : public GRAVEngine::AI::Actions::IActuator
{
public:
	directionActuator(GRAVEngine::ref<snakeAgent> agent);
	virtual ~directionActuator() = default;

	virtual std::string getName() const override;
	virtual void reset() override;
	virtual GRAVEngine::AI::Actions::actionSpec getActionSpec() override;

	virtual void onActionRecieved(GRAVEngine::AI::Actions::actionBuffer buffers) override;
	virtual void heuristic(GRAVEngine::AI::Actions::actionBuffer& buffers) override;


private:
	GRAVEngine::ref<snakeAgent> agent;
	GRAVEngine::AI::Actions::actionSpec m_ActionSpec;
};
