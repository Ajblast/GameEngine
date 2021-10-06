#pragma once

#include "common.h"

namespace GRAVEngine
{
	namespace AI
	{
		namespace Actions
		{
			struct GRAVAPI actionSpec
			{
			public:
				// Default constructor
				actionSpec();
				// Takes ownership of branchSizes
				actionSpec(size_t continuousActionCount, size_t branchCount, scope<size_t[]> branchSizes);
				// Delete the copy constructor
				actionSpec(const actionSpec& other);
				// Move Constructor
				actionSpec(actionSpec&& other) noexcept;
				// Move Operator
				actionSpec& operator=(actionSpec&& other) noexcept;
				actionSpec& operator=(const actionSpec& other);

				// Number of continuous actions
				inline size_t continuousActionCount() { return m_ContinuousActionCount; }
				// Number of branches
				inline size_t discreteActionCount() { return m_BranchCount; }
				// Get the total discrete action count
				inline size_t totalDiscreteActionCount()
				{
					size_t retVal = 0;
					for (size_t i = 0; i < m_BranchCount; i++)
						retVal += m_BranchSizes[i];

					return retVal;
				}

				inline scope<size_t[]>& branchSizes() { return m_BranchSizes; }
			private:
				size_t m_ContinuousActionCount;
				size_t m_BranchCount;			// Number of branches
				scope<size_t[]> m_BranchSizes;	// Used for discrete actions
			};
		}
	}
}