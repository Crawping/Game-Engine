#include <assert.h>
#include "MemoryPool.h"
inline Engine::MemoryPool::MemoryPool(const char* i_pMemPointer,BitStorage* i_bitptr, const unsigned int i_DataSize, const unsigned int i_DataCount) : m_pMemoryPool(i_pMemPointer), m_pFlagData(i_bitptr),m_DataSize(i_DataSize), m_DataCount(i_DataCount)
{
	assert(i_pMemPointer != nullptr);
	m_PoolCount=0;
}
/*inline Engine::MemoryPool::~MemoryPool()
{
	delete [] m_pMemoryPool;					//Deletes the pool
}*/