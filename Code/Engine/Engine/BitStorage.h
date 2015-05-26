#ifndef BITSTORAGE_H
#define BITSTORAGE_H
#include <assert.h>
#include <stdlib.h>
#include <math.h>
namespace Engine
{
	class BitStorage
	{
		unsigned int* m_pBitPointer;
		unsigned int m_NumberOf32Bits;
		unsigned int m_BitCount;
		BitStorage(const void* , const int );
		
	public:
		//inline ~BitStorage();

		static BitStorage* allocateBitMemory(const unsigned int);	
		static BitStorage* allocateBitMemory(const unsigned int , const void*,size_t& );//Allocates memory needed to store the given number of bits
		unsigned int getFirstFreeBit();									//Gives the index of the first free bit, starting from index 0
		unsigned int getAndSetFirstFreeBit();							//Gives the index of the first free bit and sets it
		unsigned int getFirstSetBit();									//Gives the index of the first set bit
		void resetBit(const unsigned int);									//Resets the value of the given index
		void showArrangement();											//Prints the int value of the bytes used for storage
		void releaseBitStorage();
	};
}
#include "BitStorage.inl"
#endif