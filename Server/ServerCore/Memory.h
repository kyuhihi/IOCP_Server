#pragma once
#include "Allocator.h"

class MemoryPool;

/*-------------------
	MemoryManager
-------------------*/

class MemoryManager
{//메모리 풀을 총괄한다.
	enum
	{//~1024까지 32단위, ~2048 128단위, ~4096까지 256단위
		POOL_COUNT = (1024/ 32) + (1024 / 128) + (2048 / 256),
		MAX_ALLOC_SIZE = 4096,
	};

public:
	MemoryManager();
	~MemoryManager();

	void* Allocate(int32 size);
	void Release(void* ptr);

private:
	vector<MemoryPool*>_pools;
	// 메모리 크기에따라 메모리풀을 빠르게 찾도록 테이블화.
	MemoryPool* _poolTable[MAX_ALLOC_SIZE + 1];
};

template<typename Type, typename... Args>
Type* xnew(Args&&... args)
{
	Type* memory = static_cast<Type*>(PoolAllocator::Alloc(sizeof(Type)));
	new(memory)Type(forward<Args>(args)...); // placement new(생성자 호출 방법)
	return memory;
}

template<typename Type>
void xdelete(Type* obj)
{
	obj->~Type();
	PoolAllocator::Release(obj);
}

template<typename Type>
shared_ptr<Type> MakeShared()
{
	return shared_ptr<Type>{xnew<Type>(), xdelete<Type>()};
}