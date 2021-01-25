#include "gravpch.h"
#include "doubleBufferedAllocator.h"

void GRAVEngine::Memory::Allocators::doubleBufferedAllocator::swapBuffers()
{
	m_CurStack = (uint32)!m_CurStack;
}

void GRAVEngine::Memory::Allocators::doubleBufferedAllocator::clearCurrentBuffer()
{
	m_Stack[m_CurStack].clear();
}

void* GRAVEngine::Memory::Allocators::doubleBufferedAllocator::alloc(uint32 size)
{
	return m_Stack[m_CurStack].alloc(size);
}
