#pragma once
#include "Allocator.h"

class MemoryPool;

/*-------------------
	MemoryManager
-------------------*/

class MemoryManager
{//�޸� Ǯ�� �Ѱ��Ѵ�.
	enum
	{//~1024���� 32����, ~2048 128����, ~4096���� 256����
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
	// �޸� ũ�⿡���� �޸�Ǯ�� ������ ã���� ���̺�ȭ.
	MemoryPool* _poolTable[MAX_ALLOC_SIZE + 1];
};

template<typename Type, typename... Args>
Type* xnew(Args&&... args)
{
	Type* memory = static_cast<Type*>(xAlloc(sizeof(Type)));
	new(memory)Type(forward<Args>(args)...); // placement new(������ ȣ�� ���)
	return memory;
}

template<typename Type>
void xdelete(Type* obj)
{
	obj->~Type();
	xRelease(obj);
}