#include "pch.h"
#include "Test2D.h"
#include <imgui/imgui.h>

#include "debug/instrumentation/instrumentor.h"

#include <thread>

GRAV_JOB_ENTRY_POINT(waitJob)
{
	GRAV_PROFILE_FUNCTION();

	GRAV_LOG_LINE_INFO("Wait Job");
	int counter = 10;
	while (counter != 0)
	{
		GRAV_LOG_LINE_CRITICAL("Waiting... %i", (int)param);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		counter--;
	}
	GRAV_LOG_LINE_CRITICAL("Wait Job Ended %i", (int)param);
}


Test2D::Test2D() : layer("Test2D"), orthoCam(-1.778, 1.778, -1, 1, 0.1f, 100.0f)
{
	orthoCam.setPosition({ 0,0,0 });
	orthoCam.setRotation({ 0, 0, 0 });
}

void Test2D::onAttach()
{
}

void Test2D::onDetach()
{
}

void Test2D::onUpdate(GRAVEngine::Time::timestep ts)
{
	orthoCam.OnUpdate(ts);
	

	if (GRAVEngine::IO::Input::isKeyPressed(GRAVEngine::Keys::Space))
	{
		GRAV_LOG_LINE_INFO("%s: Space Pressed!!!", __FUNCTION__);
		GRAVEngine::application::getInstance().close();
	}

	GRAVEngine::ref<GRAVEngine::Jobs::counter> counter;

	//GRAV_LOG_LINE_INFO("Kick single wait job");
	//GRAVEngine::Jobs::declaration job = { waitJob, 0, GRAVEngine::Jobs::jobPriority::NORMAL };
	//GRAV_KICK_JOB(job, &counter);
	//GRAV_WAIT_COUNTER(counter, 0);

	//GRAV_LOG_LINE_INFO("Kick waiting jobs");
	//int jobCount = 32;
	//GRAVEngine::scope<GRAVEngine::Jobs::declaration[]> jobs = GRAVEngine::createScope<GRAVEngine::Jobs::declaration[]>(jobCount);

	//for (size_t i = 0; i < jobCount; i++)
	//	jobs[i] = GRAVEngine::Jobs::declaration(waitJob, i, GRAVEngine::Jobs::jobPriority::NORMAL);
	//GRAV_KICK_JOBS(jobs.get(), 32, &counter);
	//GRAV_WAIT_COUNTER(counter, 0);


	// Test lambda and passing value
	//GRAV_LOG_LINE_INFO("Kick lambda jobs");
	//auto lambdaJob = [](uintptr_t value)
	//{
	//	GRAV_LOG_LINE_CRITICAL("This is a test job entry point: %d", value);
	//};
	//for (size_t i = 0; i < jobCount; i++)
	//	jobs[i] = GRAVEngine::Jobs::declaration(lambdaJob, i);
	//GRAV_KICK_JOBS(jobs.get(), 32, &counter);
	//GRAV_WAIT_COUNTER(counter, 0);

	{
		static float rotation = 0.0f;
		rotation += ts * 360.0f;

		GRAVEngine::Rendering::renderer2D::beginScene(orthoCam);
		GRAVEngine::Rendering::renderer2D::drawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		GRAVEngine::Rendering::renderer2D::drawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, rotation, { 0.8f, 0.2f, 1.0f, 1.0f });

		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
				GRAVEngine::Rendering::renderer2D::drawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}
		}		
		GRAVEngine::Rendering::renderer2D::drawQuad({ 0,0 }, { 100, 100 }, { 1, 1, 1, 1 });
		GRAVEngine::Rendering::renderer2D::endScene();
	}

}

void Test2D::onImGuiRender()
{
	ImGui::Begin("Settings");

	auto stats = GRAVEngine::Rendering::renderer2D::getStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.m_DrawCalls);
	ImGui::Text("Quads: %d", stats.m_QuadCount);

	ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
	ImGui::Text("Indices: %d", stats.getTotalIndexCount());

	ImGui::End();
}

void Test2D::onEvent(GRAVEngine::Events::event& event)
{
}
