#include "MemoryManager.h"
#include <assert.h>
#include <iostream>

Engine::MemoryManager::MemoryManager( void * i_pMemoryBase, void* i_pAlignedBase,const size_t i_MemorySize,const unsigned int i_NumBlockDescriptors,Engine::MemoryPool* i_pDescriptorPool,const size_t i_usedByMemoryPool ) :
m_pOriginalBase( i_pMemoryBase ),
m_pAlignedBase( i_pAlignedBase),
m_OriginalSize( i_MemorySize ),
m_AlignedSize( RoundDownToMultipleOf( i_MemorySize - (reinterpret_cast<uintptr_t>( m_pAlignedBase ) - reinterpret_cast<uintptr_t>( m_pOriginalBase ))-1,4 ) )	,
m_pDescriptorPool(i_pDescriptorPool),
m_usedByMemoryPool( i_usedByMemoryPool),
m_GarbageCollectionCount(0)
{
	DebugOutput("MemoryManager Constructor\n");		
	m_pFreeMemoryDescriptors = reinterpret_cast<MemoryBlockDescriptor*>(m_pDescriptorPool->allocateMemory(sizeof(MemoryBlockDescriptor )));
	m_pFreeMemoryDescriptors->m_pNext = NULL;
	m_pFreeMemoryDescriptors->m_pBlockAddress = reinterpret_cast<void *>(RoundUpToMultipleOf(reinterpret_cast<uintptr_t>( m_pAlignedBase ) + m_usedByMemoryPool+ 1,4));
	m_pFreeMemoryDescriptors->m_BlockSize = m_AlignedSize - m_usedByMemoryPool;

	m_pUsedMemoryDescriptors = reinterpret_cast<MemoryBlockDescriptor*>(m_pDescriptorPool->allocateMemory(sizeof(MemoryBlockDescriptor )));
	m_pUsedMemoryDescriptors->m_BlockSize=NULL;
	m_pUsedMemoryDescriptors->m_pBlockAddress=NULL;
	m_pUsedMemoryDescriptors->m_pNext=NULL;

	for(unsigned int i=0;i<m_AlignedSize-m_usedByMemoryPool;i++)
	{
		*(reinterpret_cast<unsigned char*>(reinterpret_cast<unsigned int>(m_pFreeMemoryDescriptors->m_pBlockAddress)+i))=0xaa;
	}
	*(reinterpret_cast<unsigned char*>(reinterpret_cast<unsigned int>(m_pAlignedBase)-1))=0xfd;
	*(reinterpret_cast<unsigned char*>(reinterpret_cast<unsigned int>(m_pAlignedBase)+m_AlignedSize))=0xfd;

}

Engine::MemoryManager* Engine::MemoryManager::createManager(void * i_pMemoryBase,const unsigned int i_MemorySize,const unsigned int i_NumBlockDescriptors)
{
	assert(i_NumBlockDescriptors>=2);
	assert(i_pMemoryBase!=NULL);
	size_t usedByMemoryPool=0;
	void * AlignedBase=reinterpret_cast<void*>(Engine:: RoundUpToMultipleOf( reinterpret_cast<unsigned int>(i_pMemoryBase)+1, 4 ));
	Engine::MemoryPool* DescriptorPool = Engine::MemoryPool::createMemoryPool( AlignedBase, sizeof( Engine::MemoryBlockDescriptor ), i_NumBlockDescriptors, usedByMemoryPool );
	assert(DescriptorPool!=NULL);
	void* PlacementAddress=reinterpret_cast<void*>(Engine::RoundUpToMultipleOf(reinterpret_cast<unsigned int>(AlignedBase)+usedByMemoryPool,4));
	usedByMemoryPool+=sizeof(MemoryManager);
	return new(PlacementAddress) Engine::MemoryManager(i_pMemoryBase,AlignedBase, i_MemorySize, i_NumBlockDescriptors,DescriptorPool,usedByMemoryPool );
}

void Engine::MemoryManager::freeMemoryManagerPool()
{
	assert(*(reinterpret_cast<unsigned char*>(reinterpret_cast<unsigned int>(m_pAlignedBase)-1))==0xfd);
	assert(*(reinterpret_cast<unsigned char*>(reinterpret_cast<unsigned int>(m_pAlignedBase)+m_AlignedSize))==0xfd);

	MemoryBlockDescriptor* UsedBlock=m_pUsedMemoryDescriptors;													//Pointer manipulated during the following loop to find the block which has to be freed		
	MemoryBlockDescriptor* PreviousBlock=NULL;	
	while(UsedBlock!=NULL)
	{
		if(UsedBlock->m_pBlockAddress!=NULL)
		{
			if(*(reinterpret_cast<unsigned char*>(reinterpret_cast<unsigned int>(UsedBlock->m_pBlockAddress)-1))!=0xfd || *(reinterpret_cast<unsigned char*>(reinterpret_cast<unsigned int>(UsedBlock->m_pBlockAddress)+UsedBlock->m_BlockSize-2))!=0xfd)
			{
				std::cout<<"Access Violation. Block Guardbands Overwritten\n";
			}
		}///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		UsedBlock=UsedBlock->m_pNext;
	}

	size_t a=0;
	//m_pDescriptorPool->releaseMemoryPool(a);
	//delete m_pDescriptorPool;
	//delete(this);
	free(m_pOriginalBase);
	//delete(this);
}

void* Engine::MemoryManager::findAdequateBlock(const size_t i_size)
{
	const unsigned int GuardbandedSize=i_size+2;																				//Accounts for the Bytes used by guardbands
	DebugOutput("Checking for available block of size %i\n",GuardbandedSize);
	size_t AlignedSize=(Engine::RoundUpToMultipleOf(GuardbandedSize,4));												//Converts the size into a multiple of 4, so that all allocations have a 4 byte alignment
	MemoryBlockDescriptor* FreeBlock=m_pFreeMemoryDescriptors;															//Pointer manipulated during the following loop to find a large enough block
	MemoryBlockDescriptor* PreviousBlock=NULL;																			//Holds the address of the previous descriptor in the linked list
	do
	{
		if(FreeBlock->m_BlockSize>=AlignedSize)
		{
			DebugOutput("FoundBlock\n");
			*(reinterpret_cast<unsigned char*>(reinterpret_cast<unsigned int>(FreeBlock->m_pBlockAddress)-1))=0xfd;								//Adds a guardband byte 0xfd before the start of the block
			*(reinterpret_cast<unsigned char*>(reinterpret_cast<unsigned int>(FreeBlock->m_pBlockAddress)+AlignedSize-2))=0xfd;					//Adds a guardband byte 0xfd at the end of the block
			for(unsigned int i=0;i<AlignedSize-2;i++)
			{
				*(reinterpret_cast<unsigned char*>(reinterpret_cast<unsigned int>(FreeBlock->m_pBlockAddress)+i))=0xcd;							//Sets all bytes between the guardbands to 0xcd to indicate that they are uninitialized
			}
			MemoryBlockDescriptor* NewDescriptor=reinterpret_cast<MemoryBlockDescriptor*>(m_pDescriptorPool->allocateMemory(sizeof(MemoryBlockDescriptor)));	//Creates a new descriptor to describe the new used block
			NewDescriptor->m_pNext=m_pUsedMemoryDescriptors;
			NewDescriptor->m_pBlockAddress=FreeBlock->m_pBlockAddress;
			NewDescriptor->m_BlockSize=AlignedSize;
			m_pUsedMemoryDescriptors=NewDescriptor;																								//Makes the new descriptor the head of the used block linked list
			(FreeBlock->m_pBlockAddress)=reinterpret_cast<void*>(reinterpret_cast<unsigned int>(FreeBlock->m_pBlockAddress)+AlignedSize);		//Adjusts the size of the FreeBlock from which the new block has been allocated
			(FreeBlock->m_BlockSize)-=AlignedSize;
			if(FreeBlock->m_BlockSize==0)																										//Checks in case the free block has shrunk to 0 size
			{
				DebugOutput("FreeBlock Size Reached 0, Adjusting links\n");
				if(PreviousBlock!=NULL)																											
					PreviousBlock->m_pNext=FreeBlock->m_pNext;
				else
					m_pFreeMemoryDescriptors=FreeBlock->m_pNext;
				m_pDescriptorPool->freeMemory(FreeBlock,sizeof(MemoryBlockDescriptor));
			}
			return m_pUsedMemoryDescriptors->m_pBlockAddress;																					//Returns the address of the block
		}
		PreviousBlock=FreeBlock;																						
		FreeBlock=FreeBlock->m_pNext;	
	}
	while(FreeBlock!=NULL);

	return NULL;

}

void Engine::MemoryManager::freeBlock(void* i_UsedBlock)										
{
	if(i_UsedBlock!=NULL)
	{
		DebugOutput("FreeingBlock\n");
		MemoryBlockDescriptor* UsedBlock=m_pUsedMemoryDescriptors;													//Pointer manipulated during the following loop to find the block which has to be freed		
		MemoryBlockDescriptor* PreviousBlock=NULL;																	//Holds the address of the previous descriptor in the linked list
		m_GarbageCollectionCount++;																					//Increments the garbage collection count
		
		assert(*(reinterpret_cast<unsigned char*>(reinterpret_cast<unsigned int>(m_pAlignedBase)-1))==0xfd);			//Verify the integrity of the guardbands around the entire chunk of memory being managed
		assert(*(reinterpret_cast<unsigned char*>(reinterpret_cast<unsigned int>(m_pAlignedBase)+m_AlignedSize))==0xfd);
		do
		{
			if(UsedBlock->m_pBlockAddress==i_UsedBlock)																//Goes through the Used Linked list to find the block which has to be freed
			{
				DebugOutput("Found Valid Block To Free\n");
				if(*(reinterpret_cast<unsigned char*>(reinterpret_cast<unsigned int>(UsedBlock->m_pBlockAddress)-1))!=0xfd || *(reinterpret_cast<unsigned char*>(reinterpret_cast<unsigned int>(UsedBlock->m_pBlockAddress)+UsedBlock->m_BlockSize-2))!=0xfd)
				{
					std::cout<<"Access Violation. Block Guardbands Overwritten\n";
				}
				for(unsigned int i=0;i<((UsedBlock->m_BlockSize));i++)
				{
					*(reinterpret_cast<unsigned char*>(reinterpret_cast<unsigned int>(UsedBlock->m_pBlockAddress)+i-1))=0xfe;			//Sets the value of all bytes in the block to 0xfe
				}
	
				if(PreviousBlock!=NULL)
					PreviousBlock->m_pNext=UsedBlock->m_pNext;														//Adjusts the linked lists
				else
					m_pUsedMemoryDescriptors=UsedBlock->m_pNext;
				UsedBlock->m_pNext=m_pFreeMemoryDescriptors;
				m_pFreeMemoryDescriptors=UsedBlock;
				std::cout<<"Freeing Memory\nLargest Free Block after Freeing: "<<getLargestFreeBlock()<<"\nTotal Free Memory after Freeing: "<<getTotalFreeMemory()<<"\n\n";
				if(m_GarbageCollectionCount==5)																		//Calls the garbage collector after a particular number of free operations
				{
					m_GarbageCollectionCount=0;
					collectGarbage();
				}
			
				return;
			}
			PreviousBlock=UsedBlock;																				
			UsedBlock=UsedBlock->m_pNext;

		}
		while(UsedBlock!=NULL);
	}
}
void* Engine::MemoryManager::allocateBlock( size_t i_size )														//Finds a large enough memory block and returns it to the user
{
	DebugOutput("Allocating Block\n");								
	void* AvailableBlock=findAdequateBlock(i_size);																
	//assert(AvailableBlock!=NULL);
	std::cout<<"Allocating Memory\nLargest Free Block after Allocating : "<<getLargestFreeBlock()<<"\nTotal Free Memory after Allocating: "<<getTotalFreeMemory()<<"\n\n";
	return AvailableBlock;
}

void Engine::MemoryManager::collectGarbage()																	//Looks for and joins adjacent free blocks
{
	std::cout<<"Garbage Collector Called\n";
	//DebugOutput("Garbage Collecter Called\n");
	MemoryBlockDescriptor* CompareBlock1=m_pFreeMemoryDescriptors;
	while(CompareBlock1!=NULL)
	{
			
		MemoryBlockDescriptor* CompareBlock2=m_pFreeMemoryDescriptors;
		MemoryBlockDescriptor* PreviousBlock=NULL;
		while(CompareBlock2!=NULL)
		{
			if(reinterpret_cast<unsigned int>(CompareBlock1->m_pBlockAddress)+(CompareBlock1->m_BlockSize)==reinterpret_cast<unsigned int>(CompareBlock2->m_pBlockAddress))			
			{
				//DebugOutput("GarbageCollected\n");											//Checks whether blocks are adjacent
				CompareBlock1->m_BlockSize=(CompareBlock1->m_BlockSize)+(CompareBlock2->m_BlockSize);
				if(PreviousBlock!=NULL)
					PreviousBlock->m_pNext=CompareBlock2->m_pNext;
				else
					m_pFreeMemoryDescriptors=CompareBlock2->m_pNext;
				MemoryBlockDescriptor* DeletableBlock=CompareBlock2;
				CompareBlock2=CompareBlock2->m_pNext;
				m_pDescriptorPool->freeMemory(DeletableBlock,sizeof(MemoryBlockDescriptor));
				std::cout<<"Found Garbage\n";
			}
			else
			{
				PreviousBlock=CompareBlock2;
				CompareBlock2=CompareBlock2->m_pNext;
			}
		}
		CompareBlock1=CompareBlock1->m_pNext;
	}
	std::cout<<"Largest Free Block after GarbageCollection : "<<getLargestFreeBlock()<<"\nTotal Free Memory after GarbageCollection: "<<getTotalFreeMemory()<<"\n\n";
}
		
unsigned int Engine::MemoryManager::getTotalFreeMemory()	
{
	unsigned int FreeMemory=0;
	MemoryBlockDescriptor* FreeMemoryDescriptor=m_pFreeMemoryDescriptors;
	while(FreeMemoryDescriptor!=NULL)														//Loops through the free blocks linked list while adding the size of each free block
	{
		FreeMemory+=FreeMemoryDescriptor->m_BlockSize;
		FreeMemoryDescriptor=FreeMemoryDescriptor->m_pNext;
	}
	return FreeMemory;
}

unsigned int Engine::MemoryManager::getLargestFreeBlock()
{
	unsigned int LargestBlock=0;
	MemoryBlockDescriptor* FreeMemoryDescriptor=m_pFreeMemoryDescriptors;
	while(FreeMemoryDescriptor!=NULL)														//Loops through the free blocks linked list to find the block with the largest size
	{				
		if(FreeMemoryDescriptor->m_BlockSize>LargestBlock)
		{
			LargestBlock=FreeMemoryDescriptor->m_BlockSize;
		}
		FreeMemoryDescriptor=FreeMemoryDescriptor->m_pNext;
	}
	return LargestBlock;
}