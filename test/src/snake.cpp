#include "pch.h"
#include "snake.h"
#include <imgui/imgui.h>


//Snake::Snake() : layer("Snake"), orthoCam(-1.778, 1.778, -1, 1, 0.1f, 100.0f)
Snake::Snake() : layer("Snake"), orthoCam(90.0f, 1.778, 0.1f, 1000.0f)
{
	// For this default app, use the default algorithm factory
	GRAVEngine::scope<GRAVEngine::AI::Training::algorithmFactory> factory = GRAVEngine::createScope<GRAVEngine::AI::Training::defaultFactory>();

	// Start up the trainer
	GRAVEngine::AI::environmentManager::instance().initialize(std::move(factory));

	orthoCam.setPosition({m_WallWidth / 2, m_WallHeight / 2, 0});
	orthoCam.setRotation({ 0, 0, 0 });
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
	if (GRAVEngine::IO::Input::isKeyPressed(GRAVEngine::Keys::Space))
	{
		GRAV_LOG_LINE_INFO("%s: Space Pressed!!!", __FUNCTION__);
		GRAVEngine::application::getInstance().close();
	}

	GRAVEngine::Rendering::renderer2D::resetStats();

	orthoCam.OnUpdate(ts);

	elapsedTime += ts.getSeconds();
	m_TotalTime += ts.getSeconds();
	m_DeltaTime = ts.getSeconds();


	//m_Agent->requestDecision();
	//GRAVEngine::AI::environmentManager::instance().step();


	if (elapsedTime >= 1 / fps)
	{
		elapsedTime = 0;

		m_Agent->requestDecision();


		// Update the environment
		GRAVEngine::AI::environmentManager::instance().step();
	}

	GRAVEngine::Rendering::rendererCommand::setClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	GRAVEngine::Rendering::rendererCommand::clear();

	GRAVEngine::Rendering::renderer2D::beginScene(orthoCam);
	// Draw the fruit
	GRAVEngine::Rendering::renderer2D::drawQuad({ m_Agent->m_FruitPosition.m_X, m_Agent->m_FruitPosition.m_Y, 0 }, { 1, 1 }, m_FruitColor);

	// Draw the walls
	for (int i = -1; i <= m_WallWidth; i++)
	{
		for (int j = -1; j <= m_WallHeight; j++)
		{
			if (i == -1 || j == -1 || i == m_WallWidth || j == m_WallHeight)
				GRAVEngine::Rendering::renderer2D::drawQuad({ i, j, 0 }, { 1, 1 }, m_WallColor);
		}
	}

	for (auto it = m_Agent->m_Body.begin(); it != m_Agent->m_Body.end(); it++)
	{
		// Draw the snake
		GRAVEngine::Rendering::renderer2D::drawQuad({ it->m_X, it->m_Y, 0 }, { 1, 1 }, m_SnakeColor);
	}
	GRAVEngine::Rendering::renderer2D::endScene();

}

void Snake::onImGuiRender()
{
	ImGui::Begin("Renderer Stats");
	auto stats = GRAVEngine::Rendering::renderer2D::getStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.m_DrawCalls);
	ImGui::Text("Quads: %d", stats.m_QuadCount);

	ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
	ImGui::Text("Indices: %d", stats.getTotalIndexCount());
	ImGui::End();

	ImGui::Begin("Stats");
	ImGui::Text("Elapsed Time: %f", m_TotalTime);
	ImGui::Text("Delta Time: %f", m_DeltaTime);
	ImGui::End();

	ImGui::Begin("Camera");
	auto pos = orthoCam.getPosition();
	auto rot = orthoCam.getRotation();
	auto foc = orthoCam.getFocalPoint();
	ImGui::Text("Position: %f, %f, %f", pos.x, pos.y, pos.z);
	ImGui::Text("Rotation: %f, %f, %f", rot.x, rot.y, rot.z);
	ImGui::Text("Focal Point: %f, %f, %f", foc.x, foc.y, foc.z);
	ImGui::End();

	ImGui::Begin("Training");
	ImGui::Text("Training Statistics");
	ImGui::Text("Fruits Captured: %i", m_Agent->fruitsCaptured);
	ImGui::Text("Suicide by Backwards: %i", m_Agent->timesMovingBackwards);
	ImGui::Text("Suicide by Wall: %i", m_Agent->wallsHit);
	ImGui::Text("Suicide by Body: %i", m_Agent->bodyHit);
	ImGui::End();
}

void Snake::onEvent(GRAVEngine::Events::event& event)
{
}
