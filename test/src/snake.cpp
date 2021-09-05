#include "snake.h"
#include <imgui/imgui.h>


Snake::Snake() : layer("Snake"), orthoCam(90.0f, 1.778, 0.1f, 1000.0f) 
{
	// For this default app, use the default algorithm factory
	GRAVEngine::scope<GRAVEngine::AI::Training::algorithmFactory> factory = GRAVEngine::createScope<GRAVEngine::AI::Training::defaultFactory>();

	// Start up the trainer
	GRAVEngine::AI::environmentManager::instance().initialize(std::move(factory));
}

void Snake::onAttach()
{
	// Generate the agent
	GRAVEngine::AI::programParams params = GRAVEngine::AI::programParams(GRAVEngine::AI::inferenceDevice::CPU, "Snake AI");
	m_Agent = GRAVEngine::createRef<snakeAgent>(m_WallWidth, m_WallHeight, params);

	// Create the sensors for the agent
	std::vector<GRAVEngine::ref<GRAVEngine::AI::Sensors::ISensor>> sensors;
	sensors.push_back(GRAVEngine::createRef<directionSensor>(m_Agent));
	sensors.push_back(GRAVEngine::createRef<headSensor>(m_Agent));
	sensors.push_back(GRAVEngine::createRef<fruitSensor>(m_Agent));
	sensors.push_back(GRAVEngine::createRef<wallSensor>(m_Agent, m_WallWidth, m_WallHeight, false));
	m_Agent->initializeSensors(sensors);

	// Create the actuators for the agent
	std::vector<GRAVEngine::ref<GRAVEngine::AI::Actions::IActuator>> actuators;
	actuators.push_back(GRAVEngine::createRef<directionActuator>(m_Agent));
	m_Agent->initializeActuators(actuators);	

	// Initialize the agent
	m_Agent->safeInitialization();

	GRAVEngine::AI::environmentManager::instance().reset();
}

void Snake::onDetach()
{
}

void Snake::onUpdate(GRAVEngine::Time::timestep ts)
{
	GRAVEngine::Rendering::renderer2D::resetStats();

	//orthoCam.OnUpdate(ts);

	elapsedTime += ts.getSeconds();

	if (elapsedTime >= 1 / fps)
	{
		// Remove the 
		elapsedTime -= 1 / fps;

		m_Agent->requestDecision();


		// Update the environment
		GRAVEngine::AI::environmentManager::instance().step();
	}

	GRAVEngine::Rendering::rendererCommand::setClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
	GRAVEngine::Rendering::rendererCommand::clear();

	GRAVEngine::Rendering::renderer2D::beginScene(orthoCam);
	for (auto it = m_Agent->m_Body.begin(); it != m_Agent->m_Body.end(); it++)
	{
		// Draw the snake
		GRAVEngine::Rendering::renderer2D::drawQuad({ it->m_X, it->m_Y, 0 }, { 50, 50 }, { 1, 1, 1, 1 });
	}
	GRAVEngine::Rendering::renderer2D::endScene();

	if (GRAVEngine::IO::Input::isKeyPressed(GRAVEngine::Keys::Space))
	{
		GRAV_LOG_LINE_INFO("%s: Space Pressed!!!", __FUNCTION__);
		GRAVEngine::application::getInstance().close();
	}
}

void Snake::onImGuiRender()
{
	ImGui::Begin("Settings");

	auto stats = GRAVEngine::Rendering::renderer2D::getStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.m_DrawCalls);
	ImGui::Text("Quads: %d", stats.m_QuadCount);

	ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
	ImGui::Text("Indices: %d", stats.getTotalIndexCount());

	ImGui::End();

	ImGui::Begin("Training");
	ImGui::Text("Training Statistics");
	ImGui::End();
}

void Snake::onEvent(GRAVEngine::Events::event& event)
{
}
