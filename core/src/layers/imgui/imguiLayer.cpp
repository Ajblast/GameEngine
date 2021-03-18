#include "gravpch.h"
#include "imguiLayer.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "application.h"

#include "rendering/renderer/rendererAPI.h"

// TEMP files
#include <GLFW/glfw3.h>
#include <glad/glad.h>

// TODO: Add ImGuizmo

GRAVEngine::Layers::imguiLayer::imguiLayer() : layer("ImGuiLayer")
{
}

void GRAVEngine::Layers::imguiLayer::onAttach()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	// Enable keyboard controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;


	// Add fonts
	io.Fonts->AddFontFromFileTTF("../core/assets/fonts/opensans/OpenSans-Bold.ttf", 18.0f);
	io.FontDefault = io.Fonts->AddFontFromFileTTF("../core/assets/fonts/opensans/OpenSans-Regular.ttf", 18.0f);

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	// ImGui::StyleColorsClassic();

	// When viewports are enabled, we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Set the theme colors
	setDarkThemeColors();

	// Get the native window
	application& app = application::getInstance();
	GLFWwindow* window = static_cast<GLFWwindow*>(app.getWindow().getNativeWindow());


	ImGui_ImplGlfw_InitForOpenGL(window, true);

	// Setup Platform/Renderer bindings
	switch (Rendering::rendererAPI::getAPI())
	{
	case Rendering::rendererAPI::API::NONE:
		GRAV_ASSERT(false); // RendererAPI::None is currently not supported!
		break;
	case Rendering::rendererAPI::API::OpenGL:
		ImGui_ImplOpenGL3_Init("#version 410");
		break;
	default: 
		GRAV_ASSERT(false);
		break;
	}

}

void GRAVEngine::Layers::imguiLayer::onDetach()
{
	// Shutdown the layer context
	switch (Rendering::rendererAPI::getAPI())
	{
	case Rendering::rendererAPI::API::NONE:
		GRAV_ASSERT(false); // RendererAPI::None is currently not supported!
		break;
	case Rendering::rendererAPI::API::OpenGL:
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		break;
	default:
		GRAV_ASSERT(false);
		break;
	}

	ImGui::DestroyContext();
}

void GRAVEngine::Layers::imguiLayer::onEvent(Events::event& event)
{
	if (m_BlockEvents)
	{
		ImGuiIO& io = ImGui::GetIO();
		event.m_Handled |= event.isInCategory(Events::eventCategory::eventCategoryMouse) & io.WantCaptureMouse;
		event.m_Handled |= event.isInCategory(Events::eventCategory::eventCategoryKeyboard) & io.WantCaptureKeyboard;
	}
}

void GRAVEngine::Layers::imguiLayer::begin()
{
	// Start platform frame
	switch (Rendering::rendererAPI::getAPI())
	{
	case Rendering::rendererAPI::API::NONE:
		GRAV_ASSERT(false); // RendererAPI::None is currently not supported!
		break;
	case Rendering::rendererAPI::API::OpenGL:
		ImGui_ImplOpenGL3_NewFrame();
		break;
	default:
		GRAV_ASSERT(false);
		break;
	}

	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	// ImGuizmo::BeginFrame();
}

void GRAVEngine::Layers::imguiLayer::end()
{
	ImGuiIO& io = ImGui::GetIO();
	application& app = application::getInstance();
	io.DisplaySize = ImVec2((float)app.getWindow().getWidth(), (float)app.getWindow().getHeight());

	// Rendering
	ImGui::Render();
	switch (Rendering::rendererAPI::getAPI())
	{
	case Rendering::rendererAPI::API::NONE:
		GRAV_ASSERT(false); // RendererAPI::None is currently not supported!
		break;
	case Rendering::rendererAPI::API::OpenGL:
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		break;
	default:
		GRAV_ASSERT(false);
		break;
	}

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void GRAVEngine::Layers::imguiLayer::setDarkThemeColors()
{
	auto& colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

	// Headers
	colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Buttons
	colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Frame BG
	colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Tabs
	colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
	colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
	colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

	// Title
	colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
}
