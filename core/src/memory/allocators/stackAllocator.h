#pragma once
#include "common.h"

namespace GRAVEngine
{
	namespace Memory
	{
		namespace Allocators
		{
			// TODO: Implement stack allocator
			// TODO: Allow memory pointer relocation

			class stackAllocator
			{
			public:
				// Stack Marker: Represents the top of the stack
				// Can only roll back to a marker, not arbitrary locations
				typedef uint32 marker;

				// Construct a stack allocator with the given total size
				explicit stackAllocator(uint32 stackSize);

				// Allocate a new block of the given size from the top
				void* alloc(uint32 size);

				// Returns a marker to the current stack top
				marker getMarker();

				// Roll the stack back to a previous marker
				void freeToMarker(marker marker);

				// Clears the entire stack
				void clear();

			private:
			};
		}
	}
}