#include "Test.h"
#include <imgui/imgui.h>

Test::Test() : layer("Test"), orthoCam(-1.778, 1.778, -1, 1)
{
	orthoCam.setPosition({ 0,0,0 });
	orthoCam.setRotation(0);
}

void Test::onAttach()
{
	std::vector<std::string> tokens;

	GRAVEngine::Utils::split("", "/", tokens);

	GRAV_LOG_LINE_INFO("Token Count:%i", tokens.size());

	for (auto it = tokens.begin(); it != tokens.end(); it++)
	{
		GRAV_LOG_LINE_INFO((*it).c_str());
	}
}

void Test::onDetach()
{
}

void Test::onUpdate(GRAVEngine::Time::timestep ts)
{
	GRAVEngine::Rendering::renderer2D::resetStats();

	GRAVEngine::Rendering::rendererCommand::setClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
	GRAVEngine::Rendering::rendererCommand::clear();

	if (GRAVEngine::IO::Input::isKeyPressed(GRAVEngine::Keys::Space))
	{
		GRAV_LOG_LINE_INFO("%s: Space Pressed!!!", __FUNCTION__);
		GRAVEngine::application::getInstance().close();
	}
}

void Test::onImGuiRender()
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

void Test::onEvent(GRAVEngine::Events::event& event)
{
}
