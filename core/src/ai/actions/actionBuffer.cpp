#include "gravpch.h"
#include "actionBuffer.h"

// TODO: Fix this. The scope created for the buffer segment will be a null reference and create an unknown behavior 
GRAVEngine::AI::Actions::actionBuffer::actionBuffer() : 
	m_ContinuousActions(createRef<actionBufferSegment<float>>(createScope<float[]>(0), 0)), 
	m_DiscreteActions(createRef<actionBufferSegment<int32>>(createScope<int32[]>(0), 0))
{
}

GRAVEngine::AI::Actions::actionBuffer::actionBuffer(ref<actionBufferSegment<float>> continuousActions, ref<actionBufferSegment<int32>> discreteActions) :
	m_ContinuousActions(continuousActions), m_DiscreteActions(discreteActions)
{
	GRAV_ASSERT(m_ContinuousActions);
	GRAV_ASSERT(m_DiscreteActions);
}

GRAVEngine::AI::Actions::actionBuffer::actionBuffer(const actionBuffer& other) : m_ContinuousActions(other.m_ContinuousActions), m_DiscreteActions(other.m_DiscreteActions)
{
}

GRAVEngine::AI::Actions::actionBuffer::actionBuffer(actionBuffer&& other) noexcept : m_ContinuousActions(std::move(other.m_ContinuousActions)), m_DiscreteActions(std::move(other.m_DiscreteActions))
{
}

GRAVEngine::AI::Actions::actionBuffer& GRAVEngine::AI::Actions::actionBuffer::operator=(const actionBuffer& other)
{
	if (this != &other)
	{
		m_ContinuousActions = other.m_ContinuousActions;
		m_DiscreteActions = other.m_DiscreteActions;
	}
	return *this;
}

GRAVEngine::AI::Actions::actionBuffer& GRAVEngine::AI::Actions::actionBuffer::operator=(actionBuffer&& other) noexcept
{
	if (this != &other)
	{
		m_ContinuousActions = std::move(other.m_ContinuousActions);
		m_DiscreteActions = std::move(other.m_DiscreteActions);
	}
	return *this;
}

void GRAVEngine::AI::Actions::actionBuffer::copy(actionBuffer buffer)
{
	//GRAV_ASSERT(m_ContinuousActions != nullptr && continuousActions()->size() == buffer.continuousActions()->size());
	//GRAV_ASSERT(m_DiscreteActions != nullptr && discreteActions()->size() == buffer.discreteActions()->size());

	// Only copy continuous actions if there are any
	if (m_ContinuousActions != nullptr && buffer.m_ContinuousActions != nullptr)
		for (size_t i = 0; i < std::min(continuousActions()->size(), buffer.continuousActions()->size()); i++)
		{
			continuousActions()->operator[](i) = buffer.continuousActions()->operator[](i);
		}

	// Only copy discrete actions if there are any
	if (m_DiscreteActions != nullptr && buffer.m_DiscreteActions != nullptr)
		for (size_t i = 0; i < std::min(discreteActions()->size(), buffer.discreteActions()->size()); i++)
		{
			discreteActions()->operator[](i) = buffer.discreteActions()->operator[](i);
		}
}

//GRAVEngine::AI::Actions::actionBuffer::actionBuffer(uint32 continuousActionCount, uint32 discreteActionCount)
//{
//	actionBufferSegment<float> continuous = actionBufferSegment<float>()
//}
