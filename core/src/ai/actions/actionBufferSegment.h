#pragma once

#include "common.h"

namespace GRAVEngine
{
	namespace AI
	{
		namespace Actions
		{
			// Allows access into an underlying array without the need of copying the array
			template<typename T>
			struct actionBufferSegment
			{
			public:
				actionBufferSegment(scope<T[]>& actions, size_t count);
				actionBufferSegment(scope<T[]>& actions, size_t count, size_t offset);

				actionBufferSegment(const actionBufferSegment<T>&& other) noexcept;
				actionBufferSegment& operator=(const actionBufferSegment<T>&& other) noexcept;

				inline void clear()
				{
					memset(m_Array.get(), 0, m_Count * sizeof(T));
				}
				inline bool isEmpty() { return m_Count == 0; }

				//inline const size_t offset() const { return m_Offset; }
				inline const size_t size() const { return m_Count; }

				inline scope<T[]>& getArray() { return m_Array; }

				T& operator[](size_t index);
			private:
				//size_t m_Offset;	// Offset into the original array from where this segment starts
				size_t m_Count;		// Number of items in the segment that can be access
				scope<T[]> m_Array;	// Reference to the original array

			};

			template<typename T>
			inline actionBufferSegment<T>::actionBufferSegment(scope<T[]>& actions, size_t count) : actionBufferSegment<T>(actions, count, 0)
			{
			}

			template<typename T>
			inline actionBufferSegment<T>::actionBufferSegment(scope<T[]>& actions, size_t count, size_t offset) : m_Array(createScope<T[]>(count)), m_Count(count)/*, m_Offset(offset)*/
			{
				memcpy(m_Array.get(), actions.get() + offset, count * sizeof(T));
			}

			template<typename T>
			inline actionBufferSegment<T>::actionBufferSegment(const actionBufferSegment<T>&& other) noexcept : m_Count(other.m_Count), m_Array(std::move(other.m_Array))
			{
			}

			template<typename T>
			inline actionBufferSegment<T>& actionBufferSegment<T>::operator=(const actionBufferSegment<T> && other) noexcept
			{
				if (this != &other)
				{
					m_Count = other.m_Count;
					m_Array = std::move(other.m_Array);
				}
				return *this;
			}

			template<typename T>
			inline T& actionBufferSegment<T>::operator[](size_t index)
			{
				GRAV_ASSERT(index < m_Count);
				return m_Array[index];
			}
		}
	}
}