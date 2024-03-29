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
	//GRAVEngine::Rendering::orthographicCamera orthoCam;

	glm::vec4 m_WallColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec4 m_FruitColor = { 1.0f, 0.0f, 0.0f, 1.0f };
	glm::vec4 m_SnakeColor = { 1.0f, 1.0f, 0.0f, 1.0f };

	const static size_t agentCountX = 5;
	const static size_t agentCountY = 5;
	std::array<GRAVEngine::ref<snakeAgent>, agentCountX * agentCountY> m_Agents;

	size_t maxStep = 500;

	//GRAVEngine::ref<snakeAgent> m_Agent;
	int m_WallWidth = 15, m_WallHeight = 15;

	float fps = 60;
	float elapsedTime = 0;
	float m_TotalTime = 0;
	float m_DeltaTime = 0;
};