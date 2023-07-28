#include "pch.h"
#include "DeadLockProfiler.h"

//-------------------------
//	DeadLockProfiler
//-------------------------

void DeadLockProfiler::PushLock(const char* name)
{
	LockGuard guard(_lock);

	//아이디를 찾거나 발급한다.
	int32 lockId = 0;
	
	auto findit = _nameToId.find(name);
	if (findit == _nameToId.end())
	{//처음발견
		lockId = static_cast<int32>(_nameToId.size());
		_nameToId[name] = lockId;
		_idToName[lockId] = name;
	}
	else {
		lockId = findit->second;
	}

	if (_lockStack.empty() == false)
	{//이미 락을 잡고있던 상태였다면?

		//기존에 발견되지 않았던 케이스라면 데드락 여부를 다시 확인한다.
		const int32 prevId = _lockStack.top();
		if (lockId != prevId)
		{
			set<int32>& history = _lockHistory[prevId];
			if (history.find(lockId) == history.end())
			{
				history.insert(lockId);
				CheckCycle();
			}
		
		}

	}
	_lockStack.push(lockId);
}

void DeadLockProfiler::PopLock(const char* name)
{
	LockGuard guard(_lock);

	if (_lockStack.empty())
		CRASH("MULTIPLE UNLOCK!!");

	int32 lockId = _nameToId[name];
	if (_lockStack.top() != lockId)
	{
		CRASH("INVALID UNLOCK");
	}

	_lockStack.pop();
}

void DeadLockProfiler::CheckCycle()
{
	// 초기화쓰
	const int32 lockCount = static_cast<int32>(_nameToId.size());
	_discoveredOrder = vector<int32>(lockCount, -1);
	_discoveredCount = 0;
	_finished = vector<bool>(lockCount, false);
	_parent = vector<int32>(lockCount, -1);

	for (int32 lockId = 0; lockId < lockCount; lockId++)
	{
		Dfs(lockId);
	}

	//연산이 끝났다면 정리한다.
	_discoveredOrder.clear();
	_finished.clear();
	_parent.clear();

}

void DeadLockProfiler::Dfs(int32 here)
{
	if (_discoveredOrder[here] != -1)
	{
		return;
	}

	_discoveredOrder[here] = _discoveredCount++;
	//모든 인접정점들 순회한다.

	auto findit = _lockHistory.find(here);
	if (findit == _lockHistory.end())
	{//아직 한번도 락을 잡지 못했다.
		_finished[here] = true;
		return;
	}

	set<int32>& nextSet = findit->second;
	for (int32 there : nextSet)
	{
		//아직 방문한적이 없다면 방문할 것.
		if (_discoveredOrder[there] == -1)
		{
			_parent[there] = here;
			Dfs(there);
			continue;
		}
		// 만약 here가 there보다 먼저 발견되었다면, there는 here의 후손이다.(순방향 간선)
		if (_discoveredOrder[here] < _discoveredOrder[there])
			continue;

		//순방향도 아니고, dfs(there)가 종료되지 않았다면, there는 here의 선조이다.(역방향 간선)
		if (_finished[there] == false)
		{
			printf("%s -> %s\n", _idToName[here], _idToName[there]);
			int32 now = here;
			while (true)
			{
				printf("%s -> %s\n", _idToName[_parent[now]], _idToName[now]);
				now = _parent[now];
				if (now == there)
					break;

			}
			
			CRASH("DeadLock Detected!!");
		}
	}


	_finished[here] = true;
}
