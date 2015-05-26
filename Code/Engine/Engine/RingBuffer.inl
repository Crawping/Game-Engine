#include <assert.h>
template<typename i_type, const unsigned int i_size >
inline unsigned int Engine::RingBuffer<i_type, i_size>::size() const
{
	return m_size;
}


template<typename i_type, const unsigned int i_size >
inline unsigned int Engine::RingBuffer<i_type, i_size>::count() const
{
	return m_count;
}

template<typename i_type, const unsigned int i_size >
inline void Engine::RingBuffer<i_type, i_size>::addItem(const i_type& i_NewItem)
{
	RingArray[m_count%m_size] = i_NewItem;
	if (m_count == m_size)
		m_count = 0;
	m_count++;
	
}

template<typename i_type, const unsigned int i_size >
inline i_type Engine::RingBuffer<i_type, i_size>::operator [](const unsigned int i_index) const
{
	assert(i_index < m_size);
	return RingArray[(m_count-i_index-1)%m_size];
}

