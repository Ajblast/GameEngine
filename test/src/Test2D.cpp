#include "Test2D.h"

Test2D::Test2D() : layer("Test2D"), orthoCam(-1.778, 1.778, -1, 1)
{
	orthoCam.setPosition({ 0,0,0 });
	orthoCam.setRotation(0);
}

void Test2D::onAttach()
{
}

void Test2D::onDetach()
{
}

void Test2D::onUpdate(GRAVEngine::Time::timestep ts)
{
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
	}
//	Rendering::renderer2D::drawQuad({ 0,0 }, { 100, 100 }, { 1, 1, 1, 1 });

}

void Test2D::onImGuiRender()
{
}

void Test2D::onEvent(GRAVEngine::Events::event& event)
{
}
