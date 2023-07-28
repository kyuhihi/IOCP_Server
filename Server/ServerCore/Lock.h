#pragma once
#include "Types.h"

//-------------------------
//	ReadWriter SpinLock
//-------------------------

//---------------------------------------------------------------------------
//[WWWWWWWW][WWWWWWWW][RRRRRRRR][RRRRRRRR]
// W: WirteFlag (Exclusive Lock Owner ThreadId) 지금 락을 획득한 ThreadId
// R: ReadFlag (Shared Lock Count) 공유하며 사용하고있는 리드카운트
//---------------------------------------------------------------------------

// W -> R (O)
// R -> W (X)

class Lock
{
	enum : uint32
	{
		ACQUIRE_TIMEOUT_TICK = 10000,
		MAX_SPIN_COUNT = 5000,
		WRITE_THREAD_MASK = 0xFFFF'0000,
		READ_COUNT_MASK = 0x0000'FFFF,
		EMPTY_FLAG = 0x0000'0000,
	};

public:
	void WriteLock();
	void WriteUnLock();
	void ReadLock();
	void ReadUnLock();

private:
	Atomic<uint32> _lockFlag = EMPTY_FLAG;
	uint16 _writeCount = 0;
};

//-------------------------
//	LockGuards
//-------------------------


class ReadLockGuard
{
public:
	ReadLockGuard(Lock& lock) : _lock(lock) { _lock.ReadLock(); }
	~ReadLockGuard() { _lock.ReadUnLock(); }

private:
	Lock& _lock;
};

class WriteLockGuard
{
public:
	WriteLockGuard(Lock& lock) : _lock(lock) { _lock.WriteLock(); }
	~WriteLockGuard() { _lock.WriteUnLock(); }

private:
	Lock& _lock;
};