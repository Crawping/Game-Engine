#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H
//All functionality in header file for now :/
#include <assert.h>
#include "MemoryPool.h"
#include "DebugLog.h"
#include "Utils.h"

namespace Engine
{
	struct MemoryBlockDescriptor
	{
		struct MemoryBlockDescriptor * m_pNext;

		void * 				m_pBlockAddress;
		size_t				m_BlockSize;
	
	};
	class MemoryManager//Make copy constructor private
	{
		MemoryManager(MemoryManager&);
		const MemoryManager operator=(const MemoryManager&);
		size_t m_usedByMemoryPool;
		void* m_pOriginalBase;																	//Holds the value of the address of the memory block provided by the user
		void* m_pAlignedBase;																	//Holds the value of the address with a 4 byte alignment
		unsigned int m_OriginalSize;															//The size of the memory block provided by the user
		unsigned int m_AlignedSize;																//The size of the memory aligned memory block
		unsigned int m_GarbageCollectionCount;													//Decides how frequently garbage is collected
		MemoryPool* m_pDescriptorPool;
		MemoryBlockDescriptor* m_pFreeMemoryDescriptors;										//A linked list of descriptors holding information about free blocks
		MemoryBlockDescriptor* m_pUsedMemoryDescriptors;										//A linked list of descriptors holding information about used blocks
		MemoryManager (void * ,void* ,const size_t ,const unsigned int, MemoryPool*,const size_t );					//A private constructor
	public:
		static MemoryManager* createManager(void * ,const unsigned int ,const unsigned int );	
		void* allocateBlock( size_t i_size );													//Finds a suitable block of memory and returns it to the user
		void* findAdequateBlock(size_t i_size);													//Searches for a large enough block, and makes necessary changes to the linked lists
		void freeBlock(void* i_UsedBlock);														//Moves the block descriptor from the free list to the used list
		void collectGarbage();																	//Finds adjacent free blocks and merges them			
		unsigned int getTotalFreeMemory();														//Calculate total free memory available
		unsigned int getLargestFreeBlock();														//Calculate the size of the largest memory block available
		void freeMemoryManagerPool();															//Frees up the entire memory pool
		
	};
}
#endif