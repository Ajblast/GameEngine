#pragma once

// These are items that should be accessible to applications

// Common item that are used everywhere and should always be accessible
#include "common.h"

#include "application.h"
#include "layers/layer.h"
#include "logging/logging.h"
#include "assertion.h"

// Time
#include "time/stopwatch.h"
#include "time/timestep.h"

// Input
#include "io/input.h"
#include "io/keyCodes.h"
#include "io/mouseCodes.h"

// ImGui
#include "layers/imgui/imguiLayer.h"

// Renderer
#include "rendering/renderer/renderer.h"
#include "rendering/renderer/renderer2D.h"
#include "rendering/renderer/rendererCommand.h"

#include "rendering/vertexArray.h"
#include "rendering/buffers/vertexBuffer.h"
#include "rendering/buffers/indexBuffer.h"
#include "rendering/buffers/bufferLayout.h"

#include "rendering/textures/texture.h"
#include "rendering/textures/texture2D.h"

#include "rendering/cameras/orthographicCamera.h"
