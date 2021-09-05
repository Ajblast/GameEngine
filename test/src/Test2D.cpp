#include "Test2D.h"
#include <imgui/imgui.h>

Test2D::Test2D() : layer("Test2D"), orthoCam(-1.778, 1.778, -1, 1)
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
	//orthoCam.OnUpdate(ts);
	
	GRAVEngine::Rendering::renderer2D::resetStats();

	GRAVEngine::Rendering::rendererCommand::setClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
	GRAVEngine::Rendering::rendererCommand::clear();

	if (GRAVEngine::IO::Input::isKeyPressed(GRAVEngine::Keys::Space))
	{
		GRAV_LOG_LINE_INFO("%s: Space Pressed!!!", __FUNCTION__);
		GRAVEngine::application::getInstance().close();
	}

	{
		static float rotation = 0.0f;
		rotation += ts * 360.0f;

		GRAVEngine::Rendering::renderer2D::beginScene(orthoCam);
		GRAVEngine::Rendering::renderer2D::drawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		GRAVEngine::Rendering::renderer2D::drawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, rotation, { 0.8f, 0.2f, 1.0f, 1.0f });
		GRAVEngine::Rendering::renderer2D::endScene();

		GRAVEngine::Rendering::renderer2D::beginScene(orthoCam);
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
