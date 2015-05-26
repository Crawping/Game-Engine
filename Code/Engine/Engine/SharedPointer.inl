#include "SharedPointer.h"
#include <assert.h>
#include "DebugLog.h"

template<class T>
inline Engine::SharedPointer<T>::SharedPointer(T* i_pPointer): m_pPointer(i_pPointer), m_pReferenceCount(new(unsigned int))
{
	assert(m_pReferenceCount != NULL);
	(*m_pReferenceCount)=1;
#ifdef _DEBUG
	//DebugOutput("New SharedPointer Created, References= %i \n",*m_pReferenceCount);
#endif
}

template<class T>
inline Engine::SharedPointer<T>::SharedPointer(const SharedPointer<T>& i_other): m_pPointer(i_other.m_pPointer), m_pReferenceCount(i_other.m_pReferenceCount)
{
	assert(m_pReferenceCount !=NULL);
	(*m_pReferenceCount)++;
#ifdef _DEBUG
	//DebugOutput("SharedPointer Copied, References= %i \n",*m_pReferenceCount);
#endif
}


template<class T>
Engine::SharedPointer<T>::~SharedPointer()
{
	if(m_pPointer!=nullptr)
	{
		assert((*m_pReferenceCount)>0);
		Decrement();
#ifdef _DEBUG
		//DebugOutput("SharedPointer Destroyed \n");
#endif
	}
}

template<class T>
inline void Engine::SharedPointer<T>::Decrement(void)
{
	(*m_pReferenceCount)--;
#ifdef _DEBUG
	//DebugOutput("SharedPointer Decremented, References= %i \n",*m_pReferenceCount);
#endif
	if((*m_pReferenceCount)==0)
	{
		delete m_pReferenceCount;
		delete m_pPointer;
#ifdef _DEBUG
		//DebugOutput("Actor Destroyed \n");
#endif
	}
}

template<class T>
inline T* Engine::SharedPointer<T>::operator->() const
{
	return m_pPointer;
}


template<class T>
inline T* Engine::SharedPointer<T>::operator->()
{
	return m_pPointer;
}




template<class T>
bool Engine::SharedPointer<T>::operator==(const SharedPointer<T>& i_rhs)
{
	return m_pPointer==i_rhs.m_pPointer?true:false;
}

template<class T>
bool Engine::SharedPointer<T>::operator!=(const SharedPointer<T>& i_rhs)
{
	return m_pPointer!=i_rhs.m_pPointer?true:false;
}

template<class T>
bool Engine::SharedPointer<T>::operator!=(const T* i_rhs)
{
	return m_pPointer!=i_rhs?true:false;
}

template<class T>
void Engine::SharedPointer<T>::operator=(const SharedPointer<T>& i_rhs)
{
	if(this->m_pReferenceCount==nullptr)
	{
		m_pPointer=i_rhs.m_pPointer;
		m_pReferenceCount=i_rhs.m_pReferenceCount;
		assert(m_pReferenceCount!=NULL);
		(*m_pReferenceCount)++;
	}
	else
	{
		if(this!=&i_rhs)
		{
			Decrement();
			m_pPointer=i_rhs.m_pPointer;
			m_pReferenceCount=i_rhs.m_pReferenceCount;
			assert(m_pReferenceCount!=NULL);
			(*m_pReferenceCount)++;
		}
#ifdef _DEBUG
		//DebugOutput("SharedPointer Assignment Called, References= %i \n",*m_pReferenceCount);
#endif
	}
}

template<class T>
inline void Engine::SharedPointer<T>::ReleasePointer()
{
	Decrement();
	m_pPointer=nullptr;
	m_pReferenceCount=nullptr;
}