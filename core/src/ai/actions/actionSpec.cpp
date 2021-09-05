#include "gravpch.h"
#include "actionSpec.h"

GRAVEngine::AI::Actions::actionSpec::actionSpec() : actionSpec(0, 0, nullptr)
{
}

GRAVEngine::AI::Actions::actionSpec::actionSpec(size_t continuousActionCount, size_t branchCount, scope<size_t[]> branchSizes) : 
	m_ContinuousActionCount(continuousActionCount), m_BranchCount(branchCount), m_BranchSizes(std::move(branchSizes))
{
}

GRAVEngine::AI::Actions::actionSpec::actionSpec(const actionSpec& other) : 
	m_ContinuousActionCount(other.m_ContinuousActionCount), m_BranchCount(other.m_BranchCount)
{
	// Create the branch array
	m_BranchSizes = createScope<size_t[]>(m_BranchCount);

	// Copy over the branch sizes
	memcpy(m_BranchSizes.get(), other.m_BranchSizes.get(), m_BranchCount * sizeof(size_t));
}


GRAVEngine::AI::Actions::actionSpec::actionSpec(actionSpec&& other) noexcept :
	m_ContinuousActionCount(other.m_ContinuousActionCount), m_BranchCount(other.m_BranchCount), m_BranchSizes(std::move(other.m_BranchSizes))
{
}

GRAVEngine::AI::Actions::actionSpec& GRAVEngine::AI::Actions::actionSpec::operator=(actionSpec&& other) noexcept
{
	if (this != &other)
	{
		// Move over the data
		m_ContinuousActionCount = other.m_ContinuousActionCount;
		m_BranchCount = other.m_BranchCount;
		m_BranchSizes = std::move(other.m_BranchSizes);
	}
	return *this;
}

GRAVEngine::AI::Actions::actionSpec& GRAVEngine::AI::Actions::actionSpec::operator=(const actionSpec& other)
{
	if (this != &other)
	{
		// Move over the data
		m_ContinuousActionCount = other.m_ContinuousActionCount;
		m_BranchCount = other.m_BranchCount;

		// Create the branch array
		m_BranchSizes = createScope<size_t[]>(m_BranchCount);

		// Copy over the branch sizes
		memcpy(m_BranchSizes.get(), other.m_BranchSizes.get(), m_BranchCount * sizeof(size_t));
	}
	return *this;
}
