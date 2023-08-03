#include "pch.h"
#include "MemoryPool.h"

/*-------------------
	MemeoryPool
-------------------*/

MemoryPool::MemoryPool(int32 allocSize) : _allocSize(allocSize)
{

}

MemoryPool::~MemoryPool()
{
	while (_queue.empty() == false)
	{
		MemoryHeader* header = _queue.front();
		_queue.pop();
		::free(header);
	}
}

void MemoryPool::Push(MemoryHeader* ptr)
{
	WRITE_LOCK;
	ptr->allocSize = 0;
	//Pool에 메모리 반납.
	_queue.push(ptr);

	_allocCount.fetch_sub(1);// 돌아오면 뺀다.
}

MemoryHeader* MemoryPool::Pop()
{
	MemoryHeader* header = nullptr;
	{
		WRITE_LOCK;
		
		//Pool에 여분이 있는지?
		if (_queue.empty() == false)
		{//있으면 하나 꺼내미
			header = _queue.front();
			_queue.pop();

		}
	}

	if (header == nullptr)
	{//없음 새로만듦.
		header = reinterpret_cast<MemoryHeader*>(::malloc(_allocSize));
	}
	else
	{
		ASSERT_CRASH(header->allocSize == 0);
	}
	_allocCount.fetch_add(1);

	return header;
}
