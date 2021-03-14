#pragma once

#include "common.h"
#include "bufferElement.h"

namespace GRAVEngine
{
	namespace Rendering
	{
		// A buffer that has multiple elements that can be of different types and sizes.
		// Describes the layout of a buffer of memory
		// Tied directly to a vertex buffer
		class bufferLayout
		{
		public:
			bufferLayout() {}
			bufferLayout(std::initializer_list<bufferElement> elements) : m_Elements(elements)
			{
				calculateOffsetAndStride();
			}

			// Get the layout stride
			uint32 getStride() const { return m_Stride; }
			// Get the elements of the buffer
			const std::vector<bufferElement>& getElements() const { return m_Elements; }

			std::vector<bufferElement>::iterator begin() { return m_Elements.begin(); }
			std::vector<bufferElement>::iterator end() { return m_Elements.end(); }
			std::vector<bufferElement>::const_iterator begin() const { return m_Elements.begin(); }
			std::vector<bufferElement>::const_iterator end() const { return m_Elements.end(); }
		private:
			// Calculate the offset and stride of the buffer
			void calculateOffsetAndStride()
			{
				size_t offset = 0;
				m_Stride = 0;

				// For each element in this buffer
				for (auto& element : m_Elements)
				{
					element.m_Offset = offset;	// Set the element's offset
					offset += element.m_Size;	// Increase the offset for next element
					m_Stride += element.m_Size;	// Increase the total stride of the entire layout
				}
			}
		private:
			std::vector<bufferElement> m_Elements;	// The elements
			uint32 m_Stride = 0;					// Stride of the entire buffer
		};
	}
}