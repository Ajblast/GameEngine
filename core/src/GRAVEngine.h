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
#include "io/input/input.h"
#include "io/input/keyCodes.h"
#include "io/input/mouseCodes.h"

// ImGui
#include "layers/imgui/imguiLayer.h"

// Renderer
#include "rendering/renderer/renderer.h"
#include "rendering/renderer/renderer2D.h"
#include "rendering/renderer/renderer3D.h"
#include "rendering/renderer/rendererCommand.h"

#include "rendering/vertexArray.h"
#include "rendering/buffers/vertexBuffer.h"
#include "rendering/buffers/indexBuffer.h"
#include "rendering/buffers/bufferLayout.h"

#include "rendering/textures/texture.h"
#include "rendering/textures/texture2D.h"

#include "rendering/cameras/orthographicCamera.h"

// AI
#include "ai/agent.h"
#include "ai/agentParams.h"
#include "ai/agentEpisodeId.h"
#include "ai/programParams.h"
#include "ai/inferenceDevice.h"

#include "ai/actions/actionBuffer.h"
#include "ai/actions/IActuator.h"

#include "ai/agentPrograms/IAgentProgram.h"

#include "ai/sensors/ISensor.h"
#include "ai/sensors/bufferSensor.h"
#include "ai/sensors/vectorSensor.h"
#include "ai/sensors/observationParams.h"
#include "ai/sensors/observationWriter.h"

#include "ai/environmentManager.h"
#include "ai/training/trainerController.h"
#include "ai/training/trainer.h"
#include "ai/training/algorithmFactory.h"
#include "ai/training/algorithmType.h"
#include "ai/training/hyperparameters.h"
#include "ai/training/ITrainingAlgorithm.h"
#include "ai/training/networkSettings.h"

#include "ai/training/algorithms/PPO.h"
#include "ai/training/defaultFactory.h"

#include "ai/models/model.h"
#include "ai/models/actorcritic/actorCritic.h"


// Utilities
#include "utils/utils.h"
