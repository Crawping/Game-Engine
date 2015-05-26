#ifndef NAMEDBITSET_H
#define NAMEDBITSET_H

#include "HashedString.h"

namespace Engine
{

	template<typename T>
	class NamedBitSet
	{
		static const unsigned int NumBits = sizeof(T) * 8; // num bytes * 8 bits / byte

		HashedString m_BitNames[NumBits];
		T m_CollisionMask[NumBits];

	public:
		NamedBitSet()
		{
			for(unsigned int i=0;i<NumBits;i++)
			{
				m_CollisionMask[i]=0;
			}
		}
		unsigned int getBitIndex( const HashedString & i_Name );
		bool findBitIndex( const HashedString & i_Name, unsigned & i_BitIndex );

		void setCollisionMask(unsigned int,T );
		T getCollisionMask(unsigned int);

		T getBitMask( const HashedString & i_Name );
		bool findBitMask( const HashedString & i_Name, T & i_BitMask );
	} ;

} 

#include "NamedBitSet.inl"

#endif 