#pragma once

#include "common.h"
#include "stackAllocator.h"

namespace GRAVEngine
{
	namespace Memory
	{
		namespace Allocators
		{
			// TODO: Allow memory pointer relocation

			class doubleBufferedAllocator
			{
				uint32 m_CurStack;
				stackAllocator m_Stack[2];

			public:
				// Swap the buffers
				void swapBuffers();

				// Clear the current buffer
				void clearCurrentBuffer();

				// Allocate size bytes in the current buffer
				void* alloc(uint32 size);
			};
		}
	}
}