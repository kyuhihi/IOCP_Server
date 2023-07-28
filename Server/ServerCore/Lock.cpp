#include "pch.h"
#include "Lock.h"
#include "CoreTLS.h"
#include "DeadLockProfiler.h"

void Lock::WriteLock(const char* name)
{
#ifdef _DEBUG
	GDeadLockProfiler->PushLock(name);
#endif
	//동일한 쓰레드가 소유하고있다면 무조건 성공하도록.
	const uint32 lockThreadId = (_lockFlag.load() & WRITE_THREAD_MASK) >> 16;
	if (LThreadId == lockThreadId)
	{
		_writeCount++;
		return;
	}

	//아무도 소유(Write) 및 공유(Read)하고 있지 않을때(초기값 Empty Flag일때), 경합해서 소유권을 얻는다.
	//위 한 문장의 의사코드:
	/*if (_lockFlag == EMPTY_FLAG)// 멀티 쓰레드 방식이라 if문으로 체크 불가능..
	{
		const uint32 desired = ((LThreadId << 16) & WRITE_THREAD_MASK);
		_lockFlag = desired;
	}*/

	const int64 BeginTick = ::GetTickCount64();
	const uint32 desired = ((LThreadId << 16) & WRITE_THREAD_MASK);
	while (true)
	{
		for (uint32 iSpinCount = 0; iSpinCount < MAX_SPIN_COUNT; iSpinCount++)
		{
			uint32 expected = EMPTY_FLAG;
			if (_lockFlag.compare_exchange_strong(OUT expected, desired))
			{
				_writeCount++;
				return;
			}
		}
		if (GetTickCount64() - BeginTick >= ACQUIRE_TIMEOUT_TICK)
			CRASH("Write Lock Time Out!!");

		this_thread::yield();
	}

}

void Lock::WriteUnLock(const char* name)
{
#ifdef _DEBUG
	GDeadLockProfiler->PopLock(name);
#endif
	// ReadLock 다 풀기 전엔 WriteUnLock 불가능.
	if ((_lockFlag.load() & READ_COUNT_MASK) != 0)
	{
		CRASH("INVALID_UNLOCK_ORDER!");
	}
	
	const int32 lockCount = --_writeCount;
	if (lockCount == 0)
		_lockFlag.store(EMPTY_FLAG);
}

void Lock::ReadLock(const char* name)
{
#ifdef _DEBUG
	GDeadLockProfiler->PushLock(name);
#endif
	// 동일한 쓰레드가 소유하고있다면 무조건 성공.
	const uint32 lockThreadId = (_lockFlag.load() & WRITE_THREAD_MASK) >> 16;
	if (LThreadId == lockThreadId)
	{
		_lockFlag.fetch_add(1);
		return;
	}

	// 아무도 소유하고 있지 않을때(아무도 writelock을 잡고있지 않을때) 경합해서 공유 카운트를 올린다.
	const int64 BeginTick = ::GetTickCount64();
	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; spinCount++)
		{
			uint32 expected = (_lockFlag.load() & READ_COUNT_MASK);
			if (_lockFlag.compare_exchange_strong(OUT expected, expected + 1))
				return;
		}
		 
		if (GetTickCount64() - BeginTick >= ACQUIRE_TIMEOUT_TICK)
			CRASH("Read Lock Time Out!!");

		this_thread::yield();
	}
}

void Lock::ReadUnLock(const char* name)
{
#ifdef _DEBUG
	GDeadLockProfiler->PopLock(name);
#endif
	if ((_lockFlag.fetch_sub(1) & READ_COUNT_MASK) == 0)
		CRASH("MULTIPLE_UNLOCK");
		

	
}
