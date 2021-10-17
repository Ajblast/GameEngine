#include "pch.h"
#include "snake.h"
#include <imgui/imgui.h>

#include "ai/models/actorcritic/actorCritic.h"
#include "ai/training/algorithms/PPO.h"


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
	for (size_t i = 0; i < agentCountX; i++)
	{
		for (size_t j = 0; j < agentCountY; j++)
		{
			GRAVEngine::AI::programParams params = GRAVEngine::AI::programParams(GRAVEngine::AI::inferenceDevice::GPU, "Snake AI", "A:\\Development\\source\\Ajblast\\GameEngine\\test\\models");
			GRAVEngine::ref<snakeAgent> agent = GRAVEngine::createRef<snakeAgent>(m_WallWidth, m_WallHeight, params, maxStep);

			// Create the sensors for the agent
			std::vector<GRAVEngine::ref<GRAVEngine::AI::Sensors::ISensor>> sensors;
			sensors.push_back(GRAVEngine::createRef<directionSensor>(agent));
			sensors.push_back(GRAVEngine::createRef<headSensor>(agent));
			sensors.push_back(GRAVEngine::createRef<bodySizeSensor>(agent));
			sensors.push_back(GRAVEngine::createRef<fruitSensor>(agent));
			sensors.push_back(GRAVEngine::createRef<wallSensor>(agent, m_WallWidth, m_WallHeight, true));
			agent->initializeSensors(sensors);

			// Create the actuators for the agent
			std::vector<GRAVEngine::ref<GRAVEngine::AI::Actions::IActuator>> actuators;
			actuators.push_back(GRAVEngine::createRef<directionActuator>(agent));
			agent->initializeActuators(actuators);

			// Initialize the agent
			agent->safeInitialization();

			m_Agents[i + j * agentCountY] = agent;
		}
	}
	



	//GRAVEngine::scope<size_t[]> branchSizes = GRAVEngine::createScope<size_t[]>(1);
	//branchSizes[0] = 4;
	//GRAVEngine::AI::Actions::actionSpec spec = GRAVEngine::AI::Actions::actionSpec(0, 1, std::move(branchSizes));
	//GRAVEngine::AI::Training::networkSettings settings = GRAVEngine::AI::Training::networkSettings(2, 8, {
	//	GRAVEngine::AI::Sensors::observationParams(8)
	//	}, spec);
	////GRAVEngine::AI::Models::ActorCritic::actorCritic ac = GRAVEngine::AI::Models::ActorCritic::actorCritic(settings);

	//GRAVEngine::ref<GRAVEngine::AI::Training::Algorithms::ppoHyperparameters> parameters = GRAVEngine::createRef<GRAVEngine::AI::Training::Algorithms::ppoHyperparameters>();
	//GRAVEngine::AI::Training::Algorithms::PPO ppo = GRAVEngine::AI::Training::Algorithms::PPO(settings, parameters);
	//ppo.saveModel("A:\\Development\\source\\Ajblast\\GameEngine\\test\\models\\TestModel");
	//ppo.print();



	//std::cout << "Parameters" << std::endl;
	//for (auto& val : ppo.getModel().get<GRAVEngine::AI::Models::ActorCritic::actorCritic>()->parameters())
	//{
	//	std::cout << val << std::endl;
	//}
	//for (auto& val : ppo.getModel().get<GRAVEngine::AI::Models::ActorCritic::actorCritic>()->named_parameters())
	//{
	//	std::cout << val.key() << ":\n" << val.value() << std::endl;
	//}
	//for (auto& val : ppo.getOptimizer().parameters())
	//{
	//	std::cout << val << std::endl;
	//}
	//std::cout << "Buffers" << std::endl;
	//for (auto& val : ppo.getModel().get<GRAVEngine::AI::Models::ActorCritic::actorCritic>()->buffers())
	//{
	//	std::cout << val << std::endl;
	//}
	//for (auto& val : ppo.getModel().get<GRAVEngine::AI::Models::ActorCritic::actorCritic>()->named_buffers())
	//{
	//	std::cout << val.key() << ":\n" << val.value() << std::endl;
	//}




	//GRAVEngine::AI::Training::Algorithms::PPO ppo2 = GRAVEngine::AI::Training::Algorithms::PPO(settings, parameters);
	//for (auto& val : ppo2.getModel().get<GRAVEngine::AI::Models::ActorCritic::actorCritic>()->named_parameters())
	//{
	//	std::cout << val.key() << ":\n" << val.value() << std::endl;
	//}
	//for (auto& val : ppo2.getOptimizer().parameters())
	//{
	//	std::cout << val << std::endl;
	//}

	//ppo2.loadModel("A:\\Development\\source\\Ajblast\\GameEngine\\test\\models\\TestModel");
	//ppo2.print();

	//for (auto& val : ppo2.getModel().get<GRAVEngine::AI::Models::ActorCritic::actorCritic>()->named_parameters())
	//{
	//	std::cout << val.key() << ":\n" << val.value() << std::endl;
	//}
	//for (auto& val : ppo2.getOptimizer().parameters())
	//{
	//	std::cout << val << std::endl;
	//}

	//ppo2.saveModel("A:\\Development\\source\\Ajblast\\GameEngine\\test\\models\\TestModel2.pt");
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

	orthoCam.OnUpdate(ts);

	elapsedTime += ts.getSeconds();
	m_TotalTime += ts.getSeconds();
	m_DeltaTime = ts.getSeconds();


	if (elapsedTime >= 1 / fps)
	{
		elapsedTime = 0;

		for (auto it = m_Agents.begin(); it != m_Agents.end(); it++)
		{
			auto agent = (*it);
			
			// Make a new decision
			agent->requestDecision();
		}

		// Update the environment
		GRAVEngine::AI::environmentManager::instance().step();
	}

	GRAVEngine::Rendering::renderer2D::beginScene(orthoCam);

	for (size_t k = 0; k < agentCountX; k++)
	{
		for (size_t l = 0; l < agentCountY; l++)
		{
			GRAVEngine::ref<snakeAgent> agent = m_Agents[k + l * agentCountY];
			int xOffset = (agent->m_WallWidth + 2) * k;
			int yOffset = (agent->m_WallHeight + 2) * l;

			// Draw the walls
			for (int i = 0; i < agent->m_WallWidth + 2; i++)
			{
				for (int j = 0; j < agent->m_WallHeight + 2; j++)
				{
					if (i == 0 || j == 0 || i == agent->m_WallWidth + 1 || j == agent->m_WallHeight + 1)
						GRAVEngine::Rendering::renderer2D::drawQuad({ i + xOffset, j + yOffset, 0 }, { 0.9f, 0.9f }, m_WallColor);
				}
			}

			// Draw the fruit
			GRAVEngine::Rendering::renderer2D::drawQuad({ agent->m_FruitPosition.m_X + xOffset + 1, agent->m_FruitPosition.m_Y + yOffset + 1, 0 }, { 1, 1 }, m_FruitColor);

			for (auto it = agent->m_Body.begin(); it != agent->m_Body.end(); it++)
			{
				// Draw the snake
				GRAVEngine::Rendering::renderer2D::drawQuad({ it->m_X + xOffset + 1, it->m_Y + yOffset + 1, 0 }, { 1, 1 }, m_SnakeColor);
			}
		}
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

	for (size_t i = 0; i < agentCountX; i++)
	{
		ImGui::Begin(("Training: " + std::to_string(i)).c_str());
		ImGui::Text("Training Statistics");
		ImGui::Text("Current Reward: %f", m_Agents[i]->currentReward());
		ImGui::Text("Cumulative Reward: %f", m_Agents[i]->cumulativeReward());
		ImGui::Text("Fruits Captured: %i", m_Agents[i]->fruitsCaptured);
		ImGui::Text("Episode Count: %i", m_Agents[i]->completedEpisdes());
		ImGui::Text("Current Lifespan: %i", m_Agents[i]->currentLifespan);
		ImGui::Text("Suicide by Wall: %i", m_Agents[i]->wallsHit);
		ImGui::Text("Suicide by Body: %i", m_Agents[i]->bodyHit);
		ImGui::End();
	}
}

void Snake::onEvent(GRAVEngine::Events::event& event)
{
}
