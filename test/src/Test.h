#pragma once

#include "GRAVEngine.h"

class Test : public GRAVEngine::Layers::layer
{
public:
	Test();
	virtual ~Test() = default;

	virtual void onAttach() override;
	virtual void onDetach() override;

	void onUpdate(GRAVEngine::Time::timestep ts) override;
	virtual void onImGuiRender() override;
	void onEvent(GRAVEngine::Events::event& event) override;
private:
	//GRAVEngine::Rendering::orthographicCameraController m_CameraController;

	GRAVEngine::Rendering::orthographicCamera orthoCam;

	// Temp
	//Hazel::Ref<Hazel::VertexArray> m_SquareVA;
	//Hazel::Ref<Hazel::Shader> m_FlatColorShader;

	//Hazel::Ref<Hazel::Texture2D> m_CheckerboardTexture;

	//glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };


};