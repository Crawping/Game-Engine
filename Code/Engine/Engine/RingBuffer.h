#ifndef RINGBUFFER_H
#define RINGBUFFER_H
namespace Engine
{
	template<typename i_type,const unsigned int i_size=10 >
	class RingBuffer
	{
		i_type RingArray[i_size];
		unsigned int m_size;
		unsigned int m_count;
	public:
		RingBuffer():m_size(i_size),m_count(0)												//Default constructor,sets buffer values to zero
		{
			for (unsigned int loopcounter = 0; loopcounter < m_size; loopcounter++)
				RingArray[loopcounter] = 0;
		}

		void clear()													//Clears buffer
		{
			m_count = 0;
			for (unsigned int loopcounter = 0; loopcounter < m_size; loopcounter++)
				RingArray[loopcounter] = 0;
		}

		void outputBuffer() const										//Outputs values with an arrow to denote the last input
		{
			for (unsigned int loopcounter = 0; loopcounter < m_size; loopcounter++)
			{
				std::cout << RingArray[loopcounter] << "   ";										
				if (loopcounter == m_count - 1)
					std::cout << "    <-------";
				std::cout << endl;
			}
			std::cout << endl;
		}

		inline unsigned int size() const;								//Returns size
		inline unsigned int count() const;								//Returns count
		inline void addItem(const i_type&);								//Adds Items
		inline i_type operator [](const unsigned int) const;					//Returns indexed item
	};
}
#include "RingBuffer.inl"
#endif