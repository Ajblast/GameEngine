#include "gravpch.h"
#include "inferenceProgram.h"
#include "ai/sensors/bufferWriter.h"

GRAVEngine::AI::inferenceProgram::inferenceProgram(ref<Models::model> model) : m_Model(model)
{
	GRAV_PROFILE_FUNCTION();
	
	if (model == nullptr)
        throw Exceptions::invalidArgumentException("Model is null");
}

void GRAVEngine::AI::inferenceProgram::requestDecision(agentInfo info, scope<ref<Sensors::ISensor>[]>& sensors, size_t count)
{
	GRAV_PROFILE_FUNCTION();
	
	std::vector<torch::Tensor> observations;
	for (size_t i = 0; i < count; i++)
	{
		auto sensor = sensors[i];
		int64 sensorSize = sensor->getObservationParams().m_Shape.length();

		// Create a place to store the sensor data
		scope<float[]> sensorData = createScope<float[]>(sensorSize);

		// Create the writer
		Sensors::bufferWriter writer = Sensors::bufferWriter(sensorData.get(), sensorSize);

		// Write the sensor data
		sensor->write(writer);

		// Create a tensor
		torch::Tensor sensorTensor = torch::from_blob(sensorData.get(), { sensorSize }).clone();

		// Add this sensor's data to the list of data
		observations.push_back(sensorTensor);
	}

	auto acOutput = m_Model->operator->()->forward(observations);

	// Set the stored buffer value
	m_Buffer = std::get<0>(std::get<0>(acOutput)).toAgentBuffer();
}

GRAVEngine::AI::Actions::actionBuffer GRAVEngine::AI::inferenceProgram::decideAction()
{
	return m_Buffer;
}

void GRAVEngine::AI::inferenceProgram::sendToDevice(inferenceDevice device)
{
    switch (device)
    {
    case GRAVEngine::AI::inferenceDevice::CPU:
        m_Model->operator->()->to(torch::kCPU);
        break;
    case GRAVEngine::AI::inferenceDevice::GPU:
        // Send the model to the GPU if cuda core are actually available.
        m_Model->operator->()->to(torch::cuda::is_available() ? torch::kCUDA : torch::kCPU);
        break;
    default:
        break;
    }
}
