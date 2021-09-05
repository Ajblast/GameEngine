#pragma once

#include "GRAVEngine.h"
#include "rendering/cameras/editorCamera.h"
#include "snakeAI/snakeAgent.h"
#include "snakeAI/snakeSensors.h"
#include "snakeAI/snakeActuators.h"


class Snake : public GRAVEngine::Layers::layer
{
public:
	Snake();
	virtual ~Snake() = default;

	virtual void onAttach() override;
	virtual void onDetach() override;

	void onUpdate(GRAVEngine::Time::timestep ts) override;
	virtual void onImGuiRender() override;
	void onEvent(GRAVEngine::Events::event& event) override;
private:
	GRAVEngine::Rendering::editorCamera orthoCam;

	glm::vec4 m_SnakeColor = { 0.2f, 0.3f, 0.8f, 1.0f };

	GRAVEngine::AI::Models::ActorCritic::actorCritic ac;

	GRAVEngine::ref<snakeAgent> m_Agent;
	int m_WallWidth = 20, m_WallHeight = 20;

	float fps = 1;
	float elapsedTime = 0;
};