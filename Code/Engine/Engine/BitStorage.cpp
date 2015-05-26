#include "BitStorage.h"
#include <iostream>

Engine::BitStorage::BitStorage(const void* i_pBitPtr,const int i_BitNum) : m_pBitPointer(const_cast<unsigned int*> (static_cast<const unsigned int*> (i_pBitPtr))), m_BitCount(i_BitNum), m_NumberOf32Bits((unsigned int)((i_BitNum+31)/32))
{
	assert(i_pBitPtr != nullptr);
	for (unsigned int i = 0; i < ((m_BitCount + 31) / 32); i++)				//Sets all the bytes to 0x00000000
		m_pBitPointer[i] = 0;
};

Engine::BitStorage* Engine::BitStorage::allocateBitMemory(const unsigned int i_ItemCount)
{
	unsigned int NumberOf32Bits = ( (i_ItemCount + 31) / 32);	//Calculate number of 4 byte datamembers required
	void* pBitPointer = malloc(NumberOf32Bits*4);						//Allocates memory enough for storing the given bits
	if (pBitPointer == nullptr)
		return nullptr;
	return new BitStorage(pBitPointer, i_ItemCount);
}
		
Engine::BitStorage* Engine::BitStorage::allocateBitMemory(const unsigned int i_ItemCount,const void* i_ManagerPool,size_t& i_BytesUsed)
{
	unsigned int NumberOf32Bits = ( (i_ItemCount + 31) / 32);	//Calculate number of 4 byte datamembers required
	void* pBitPointer = new (const_cast<void*>(i_ManagerPool))char[NumberOf32Bits*4];						//Allocates memory enough for storing the given bits
	if (pBitPointer == nullptr)
		return nullptr;
	i_BytesUsed=NumberOf32Bits*4+sizeof(BitStorage);
	void* PlacementAddress=reinterpret_cast<void*>(reinterpret_cast<unsigned int>(i_ManagerPool)+NumberOf32Bits*4);
	return new(PlacementAddress) BitStorage(pBitPointer, i_ItemCount);
}

unsigned int Engine::BitStorage::getFirstFreeBit()
{
	unsigned int index = 0;
	for (index = 0; index < (m_NumberOf32Bits); index++)
		if (m_pBitPointer[index] != 0xffffffff)						//Finds first free block by comparing each with 0xffffffff
			break;
	assert(index <= m_NumberOf32Bits);

	unsigned int value = m_pBitPointer[index] ^ 0xffffffff;			//Converts all 1's to 0's and viceversa. So, 00001100 becomes 11110011
	unsigned int shiftcount;
	for (shiftcount = 0; shiftcount < 32; shiftcount++)				//Shifts bits to the left until the value left is 10000000..... (31 zeroes)
	{																//Calculates number of shifts required and returns that value
		if (value << shiftcount == 0x80000000)	
			break;
	}
	return index * 32 + (31 - shiftcount);
}

unsigned int Engine::BitStorage::getAndSetFirstFreeBit()
{
	unsigned int bitindex = getFirstFreeBit();
	if (bitindex > m_BitCount)
		return bitindex;
	else
	{
		int val = m_pBitPointer[ (bitindex / 32)];
		int offset = bitindex % 32;
		//std::cout << "Bit Number " << offset << " set" << std::endl;
		val = val + (0x00000001 << offset);								//Resets value by offsetting 0x000000001 and adding so that the 1 bit is in the current place
		m_pBitPointer[(bitindex / 32)] = val;
		return bitindex;
	}
}

unsigned int Engine::BitStorage::getFirstSetBit()
{
	unsigned int index = 0;
	for (index = 0; index <  ((m_BitCount + 31) / 32); index++)
	{
		if (m_pBitPointer[index] != 0x00000000)						//Finds first block which is not fully empty
			break;
	}
	unsigned int bitshift;
	for (bitshift = 0; bitshift < 32; bitshift++)
	{
		if (m_pBitPointer[index] << bitshift == 0x80000000)			//Left shift till you get 100000.....(31zeroes).
			break;													//Find number of shifts and return 
	}
	return index * 32 + (31 - bitshift);
}

void Engine::BitStorage::resetBit(const unsigned int location)
{
	unsigned int bitshift = location % 32;
	m_pBitPointer[((location) / 32)] -= (0x00000001 << bitshift);			//Resets given bit
}


void Engine::BitStorage::showArrangement()
{
	//std::cout << "Arrangement of bits after deleting:";										//Shows the unsigned int value of the bytes being used to store the state of the memory pool
	int bytes = (m_BitCount + 31) / 32;
	for (int i = 0; i < bytes; i++)
	{
		//std::cout <<"Byte "<<i<<": "<< m_pBitPointer[i] << " ";
	}
	//std::cout<<std::endl;
}

void Engine::BitStorage::releaseBitStorage()												//Deletes the Bit Memory Pool
{
	free( m_pBitPointer);
}