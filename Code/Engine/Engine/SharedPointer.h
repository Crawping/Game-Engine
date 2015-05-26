#ifndef SHAREDPOINTER_H
#define SHAREDPOINTER_H

namespace Engine
{
	template<class T>
	class SharedPointer
	{
		T* m_pPointer;
		unsigned int* m_pReferenceCount;
	public:
		inline SharedPointer(void){m_pPointer=nullptr; m_pReferenceCount=0;};
		inline SharedPointer(T* i_pPointer);
		inline SharedPointer(const SharedPointer<T>& i_other);
		inline void Decrement(void);
		inline T* operator->() const;
		inline T* operator->();
		inline bool operator==(const SharedPointer<T>& i_rhs);
		inline bool operator!=(const SharedPointer<T>& i_rhs);
		inline void ReleasePointer();
		bool operator!=(const T*);
		void operator=(const SharedPointer<T>& i_rhs);
		~SharedPointer();
		
	};
}
#include "SharedPointer.inl"
#endif
