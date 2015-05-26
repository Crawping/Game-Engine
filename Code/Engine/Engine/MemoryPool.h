#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H
#include <assert.h>
#include <iostream>
#include "BitStorage.h"
#include "DebugLog.h"

namespace Engine
{
	class MemoryPool
	{
		const char* m_pMemoryPool;
		unsigned int m_DataSize;
		unsigned int m_DataCount;
		unsigned int m_PoolCount;
		BitStorage* m_pFlagData;
		
	
		inline MemoryPool(const char* i_pMemPointer,BitStorage* i_bitptr, const unsigned int i_DataSize, const unsigned int i_DataCount);		//Constructor to set values
		
	public:

		void releaseMemoryPool();
		void releaseMemoryPool(size_t);
		static MemoryPool* createMemoryPool(const unsigned int , const unsigned int );					//Creates Memory Pool
		static MemoryPool* createMemoryPool(const void* , unsigned int , unsigned int , size_t& );
		void* allocateMemory(const unsigned int i_size);													//Returns pointer to free block of memory pool
		void freeMemory(const void* i_pMemBlock, const unsigned int i_size);								//Releases used memory
		//inline ~MemoryPool();
	};
}
#include "MemoryPool.inl"
#endif