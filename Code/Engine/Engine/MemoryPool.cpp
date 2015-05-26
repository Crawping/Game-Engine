#include "MemoryPool.h"
#include "MemoryManager.h"
#include "BitStorage.h"
#include "Actor.h"

Engine::MemoryPool* Engine::MemoryPool::createMemoryPool(const unsigned int i_DataSize, const unsigned int i_DataCount)
{
	char* pMemPtr = new char[i_DataSize*i_DataCount];								//Creates pool of appropriate size
	BitStorage* bitptr = BitStorage::allocateBitMemory(i_DataCount);
	if (pMemPtr == nullptr)
	{
		return nullptr;
	}
	if (bitptr == nullptr)
	{
		return nullptr;
	}

	return new MemoryPool(pMemPtr,bitptr, i_DataSize, i_DataCount);
}

 Engine::MemoryPool* Engine::MemoryPool::createMemoryPool(const void* i_PoolAddress,const unsigned int i_size,const unsigned int i_NumBlockDescriptors, size_t& i_BytesUsed)
{					
	BitStorage* bitptr = BitStorage::allocateBitMemory(i_NumBlockDescriptors,i_PoolAddress, i_BytesUsed);
	if (i_PoolAddress == nullptr)
	{
		return nullptr;
	}
	if (bitptr == nullptr)
	{
		return nullptr;
	}
	void* MemoryPoolAddress=reinterpret_cast<void*>(Engine::RoundUpToMultipleOf(reinterpret_cast<unsigned int>(i_PoolAddress)+i_BytesUsed,4));
	void* PlacementAddress=reinterpret_cast<void*>(Engine::RoundUpToMultipleOf(reinterpret_cast<unsigned int>(MemoryPoolAddress)+i_NumBlockDescriptors*sizeof(MemoryBlockDescriptor),4));
	i_BytesUsed+=Engine::RoundUpToMultipleOf(i_NumBlockDescriptors*sizeof(Engine::MemoryBlockDescriptor),4)+Engine::RoundUpToMultipleOf(sizeof(MemoryPool),4);
	return new(PlacementAddress) MemoryPool(reinterpret_cast<const char*>(MemoryPoolAddress),bitptr, i_size, i_NumBlockDescriptors);
}

void* Engine::MemoryPool::allocateMemory(const unsigned int i_size)
{
	assert(i_size == m_DataSize);
	unsigned int freebit = m_pFlagData->getAndSetFirstFreeBit();					//Finds first free block,sets it and returns that address
	assert(m_PoolCount < m_DataCount);
	if (freebit < m_DataCount)
	{
		m_PoolCount++;																//Keeps a track on the number of items in the pool
		return (const_cast<char*>(m_pMemoryPool) + freebit*m_DataSize);
	}
	m_pFlagData->resetBit(freebit);												//Resets Bit if allocation failed
		//DebugOutput("New Failed. Memory Pool Full \n");
	assert(1);
	return nullptr;
}

void Engine::MemoryPool::freeMemory(const void* i_pMemBlock, const unsigned int i_size)		//Resets flag assosiated with a memory block
{
	if (i_pMemBlock == nullptr)																//Outputs to DebugLog if called on nullptr
	{
		DebugOutput("Delete Failed. Null Pointer \n");
		return;
	}
	assert(i_pMemBlock != nullptr);
	m_pFlagData->resetBit((reinterpret_cast<unsigned int> (i_pMemBlock) - reinterpret_cast<unsigned int> (m_pMemoryPool)) / i_size);
	m_pFlagData->showArrangement();
	if (m_PoolCount==1)																		//When called for last item in pool,deletes pool
		releaseMemoryPool();
	else
		m_PoolCount--;
}

void Engine::MemoryPool::releaseMemoryPool()
{
	m_pFlagData->releaseBitStorage();														//Deletes the BitStorage Pool
	delete m_pFlagData;																		//Deletes the Pointer to BitPool which was assigned on the heap
	delete [] m_pMemoryPool;																//Deletes Memory Pool				
	delete Engine::Actor::m_pActorPool;														//Deletes the static pointer to the Memory Pool
}

void Engine::MemoryPool::releaseMemoryPool(size_t i_BytesUsed)
{
	m_pFlagData->releaseBitStorage();														//Deletes the BitStorage Pool
	delete m_pFlagData;																		//Deletes the Pointer to BitPool which was assigned on the heap
	//delete [] m_pMemoryPool;																//Deletes Memory Pool				
	//delete Engine::Actor::m_pActorPool;														//Deletes the static pointer to the Memory Pool
}