#pragma once

#include "common.h"
#include "IActuator.h"

namespace GRAVEngine
{
	namespace AI
	{
		namespace Actions
		{
			// Grouping of actuators
			class actuatorList
			{
			public:
				inline const bool executable() const { return m_Executable; }

				inline const size_t discreteActionCount() const { return m_DiscreteCount; }
				inline const size_t totalDiscreteActionCount() const { return m_TotalDiscreteActionsCount; }
				inline const size_t continuousActionCount() const { return m_ContinuousCount; }
				inline const size_t actionCount() const { return discreteActionCount() + continuousActionCount(); }
				inline const actionBuffer buffer() const { return m_ActionBuffer; }

				const actionSpec totalActionSpec();

			public:
				actuatorList();
				// Make a list with the capacity
				actuatorList(size_t capacity);
				// Get the actuators ready for execution
				void readyForExecution();
				// Update all of the actuators
				void updateActions(actionBuffer actions);
				// Apply the heuristic for the actuators
				void applyHeuristic(actionBuffer& actions);
				
				// Execute all actuators
				void executeActions();
				// Reset the buffer to default value
				void reset();

				// Add an actuator
				void addActuator(ref<IActuator> actuator);
				// Remove an actuator
				void removeActuator(ref<IActuator> actuator);
				// Does the list contain the actuator
				bool contains(ref<IActuator> actuator);
				// Clear the actuators
				void clear();
			private:
				void readyForExecution(size_t continuousCount, size_t discreteCount, size_t totalDiscreteCount);
				actionSpec getTotalActionSpec();
				// Sort the actuators by name.
				void sortActuators(std::vector<ref<IActuator>>);
				// Validate that all actuators are unique (By name)
				void validateActuators();

				template<typename T>
				void updateActionArray(actionBufferSegment<T>& source, size_t sourceSize, actionBufferSegment<T>& destination, size_t destinationSize);

				// Add an actuator to the internal buffer
				void addToBuffer(ref<IActuator> actuator);
				// Sub an actuator to the internal buffer
				void subToBuffer(ref<IActuator> actuator);
				// Clear the buffer counts
				void clearBuffer();

			private:
				std::vector<ref<IActuator>> m_Actuators;
				actionSpec m_ActionSpec;
				actionBuffer m_ActionBuffer;
				bool m_Executable;

				scope<float[]> m_ContinuousActions;
				scope<int32[]> m_DiscreteActions;

				size_t m_ContinuousCount;
				size_t m_DiscreteCount;
				size_t m_TotalDiscreteActionsCount;
			};

			template<typename T>
			inline void actuatorList::updateActionArray(actionBufferSegment<T>& source, size_t sourceSize, actionBufferSegment<T>& destination, size_t destinationSize)
			{
				// Clear the destination if the source is empty
				if (sourceSize == 0)
					destination.clear();
				else
				{
					GRAV_ASSERT(sourceSize == destinationSize);

					// Copy the memory over
					memcpy(destination.getArray().get() /*+ destination.offset()*/, source.getArray().get() /*+ source.offset()*/, sourceSize * sizeof(T));
				}
			}
		}
	}
}