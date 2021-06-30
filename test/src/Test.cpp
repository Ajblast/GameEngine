#include "Test.h"
#include <imgui/imgui.h>

#include "io/io.h"

Test::Test() : layer("Test"), orthoCam(90.0f, 1.778, 0.1f, 1000.0f)
//Test::Test() : layer("Test"), orthoCam(1.778, 1.778, -1, 1)
{
	orthoCam.setPosition({ 0,3,10 });
	orthoCam.setRotation({ 0, 0, 0 });
}

void Test::onAttach()
{
	GRAVEngine::Tools::Importing::objImporter importer = GRAVEngine::Tools::Importing::objImporter();

	GRAV_LOG_LINE_INFO(GRAVEngine::IO::currentPath().string().c_str());
	importer.canRead("Cube.obj", true);
	sc = importer.readFile("Cube.obj");



	//std::vector<std::string> tokens;

	//GRAVEngine::Utils::split("", "/", tokens);

	//GRAV_LOG_LINE_INFO("Token Count:%i", tokens.size());

	//for (auto it = tokens.begin(); it != tokens.end(); it++)
	//{
	//	GRAV_LOG_LINE_INFO((*it).c_str());
	//}
}

void Test::onDetach()
{
}

void Test::onUpdate(GRAVEngine::Time::timestep ts)
{
	orthoCam.OnUpdate(ts);

	GRAVEngine::Rendering::renderer3D::resetStats();

	GRAVEngine::Rendering::rendererCommand::setClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
	GRAVEngine::Rendering::rendererCommand::clear();

	GRAVEngine::Rendering::renderer3D::beginScene(orthoCam);

	GRAVEngine::ref<GRAVEngine::mesh> testMesh = GRAVEngine::createRef<GRAVEngine::mesh>();
	testMesh->m_Vertices = GRAVEngine::createScope<GRAVEngine::vec3[]>(4);
	testMesh->m_VertexCount = 4;
	testMesh->m_FaceCount = 2;
	testMesh->m_Vertices[0] = { -0.5f, -0.5f, 0.0f, };
	testMesh->m_Vertices[1] = {  0.5f, -0.5f, 0.0f, };
	testMesh->m_Vertices[2] = {  0.5f,  0.5f, 0.0f, };
	testMesh->m_Vertices[3] = { -0.5f,  0.5f, 0.0f, };

	testMesh->m_Faces = GRAVEngine::createScope<GRAVEngine::face[]>(2);
	testMesh->m_Faces[0].m_VertexCount = 3;
	testMesh->m_Faces[0].m_Vertices = GRAVEngine::createScope<GRAVEngine::vertexIndex[]>(3);
	testMesh->m_Faces[0].m_Vertices[0] = 0;
	testMesh->m_Faces[0].m_Vertices[1] = 1;
	testMesh->m_Faces[0].m_Vertices[2] = 2;
	testMesh->m_Faces[1].m_VertexCount = 3;
	testMesh->m_Faces[1].m_Vertices = GRAVEngine::createScope<GRAVEngine::vertexIndex[]>(3);
	testMesh->m_Faces[1].m_Vertices[0] = 2;
	testMesh->m_Faces[1].m_Vertices[1] = 3;
	testMesh->m_Faces[1].m_Vertices[2] = 0;

	GRAVEngine::Rendering::renderer3D::drawMesh({ 0, 0, 0 }, sc->m_Meshes[0], { 1, 1, 1, 1 });
	//GRAVEngine::Rendering::renderer3D::drawMesh({ 0, 0, 0 }, testMesh, { 1, 1, 1, 1 });

	GRAVEngine::Rendering::renderer3D::endScene();


	if (GRAVEngine::IO::Input::isKeyPressed(GRAVEngine::Keys::Space))
	{
		GRAV_LOG_LINE_INFO("%s: Space Pressed!!!", __FUNCTION__);
		GRAVEngine::application::getInstance().close();
	}
}

void Test::onImGuiRender()
{
	ImGui::Begin("Settings");

	auto stats = GRAVEngine::Rendering::renderer3D::getStats();
	ImGui::Text("Renderer3D Stats:");
	ImGui::Text("Draw Calls: %d", stats.m_DrawCalls);
	ImGui::Text("Triangles: %d", stats.m_TriangleCount);

	ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
	ImGui::Text("Indices: %d", stats.getTotalIndexCount());

	ImGui::End();
}

void Test::onEvent(GRAVEngine::Events::event& event)
{
}
