#pragma once

#include "common.h"

namespace GRAVEngine
{
	namespace Memory
	{
		namespace Allocators
		{
			// TODO: Have a way to track all the allocations
			// TODO: Allow memory pointer relocation

			// Shift the given address upwards if/as necessary to ensure it is aligned to the given number of bytes.
			inline uintptr_t alignAddress(uintptr_t addr, size_t align)
			{
				const size_t mask = align - 1;
				GRAV_ASSERT((align & mask) == 0); // Must be power of 2
				return (addr + mask) & ~mask;
			}

			// Shift the given pointer upwards if/as necessary to ensure it is aligned to the given number of bytes
			template<typename T>
			inline T* alignPointer(T* ptr, size_t align)
			{
				const uintptr_t addr = reinterpret_cast<uintptr_t> (ptr);
				const uintptr_t addrAligned = alignAddress(addr, align);
				return reinterpret_cast<T*>addrAligned);
			}

			// Aligned allocation function. Important: "align" must be a power of 2
			void* allocAligned(size_t bytes, size_t align)
			{
				// Allocate 'align' more bytes than we need.
				size_t actualBytes = bytes + align;

				// Allocate unaligned block.
				// TODO: overload new operator to allow tracking of memory allocations
				uint8* pRawMem = new uint8[actualBytes];

				// Align the block. If no alignment occurred, shift it up the full 'align' bytes so we always have room to store the shift.
				uint8* pAlignMem = alignPointer(pRawMem, align);
				if (pAlignMem == pRawMem)
					pAlignMem += align;

				// Determine the shift, and store it. Works up to 235 bytes alignment
				ptrdiff_t shift = pAlignMem - pRawMem;
				GRAV_ASSERT(shift > 0 && shift <= 256);

				pAlignMem[-1] = static_cast<uint8>(shift & 0xFF);

				return pAlignMem;
			}

			// Free an aligned pointer
			void freeAligned(void* pMem)
			{
				if (pMem)
				{
					// Convert to uint8 pointer
					uint8* pAlignedMem = reinterpret_cast<uint8*>(pMem);

					// Extract the shift.
					ptrdiff_t shift = pAlignedMem[-1];
					if (shift == 0)
						shift = 256;

					// Back up to the actual allocated address, and array-delete it.
					uint8* pRawMem = pAlignedMem - shift;

					// TODO: overload delete operator to track memory allocations
					delete[] pRawMem;
				}
			}

			// TODO: Implement free functions into alignedAllocator class
			class alignedAllocator
			{
			public:
				// Construct aligned allocator with the given total size
				explicit alignedAllocator(uint32 bufferSize);

				// Allocate a new aligned block of the given size
				void* alloc(size_t bytes, size_t align);

				// Free aligned pointer
				void free(void* pMem);

				// Clear the entire buffer
				void clear();
			};
		}
	}
}