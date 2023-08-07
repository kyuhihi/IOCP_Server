#include "pch.h"
#include "MemoryPool.h"

/*-------------------
	MemeoryPool
-------------------*/

MemoryPool::MemoryPool(int32 allocSize) : _allocSize(allocSize)
{
	::InitializeSListHead(&_header);
}

MemoryPool::~MemoryPool()
{
	while (MemoryHeader* memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&_header)))
	{
		::_aligned_free(memory);
	}
}

void MemoryPool::Push(MemoryHeader* ptr)
{
	ptr->allocSize = 0;

	//Pool에 메모리 반납.
	::InterlockedPushEntrySList(&_header, static_cast<PSLIST_ENTRY>(ptr));

	_useCount.fetch_sub(1);// 돌아오면 뺀다.
	_reserveCount.fetch_add(1);
}

MemoryHeader* MemoryPool::Pop()
{
	MemoryHeader* memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&_header));


	if (memory == nullptr)
	{//없음 새로만듦.
		memory = reinterpret_cast<MemoryHeader*>(::_aligned_malloc(_allocSize,SLIST_ALIGNMENT));
	}
	else
	{
		ASSERT_CRASH(memory->allocSize == 0);
		_reserveCount.fetch_sub(1);

	}
	_useCount.fetch_add(1);

	return memory;
}
