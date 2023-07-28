#include "pch.h"
#include "DeadLockProfiler.h"

//-------------------------
//	DeadLockProfiler
//-------------------------

void DeadLockProfiler::PushLock(const char* name)
{
	LockGuard guard(_lock);

	//���̵� ã�ų� �߱��Ѵ�.
	int32 lockId = 0;
	
	auto findit = _nameToId.find(name);
	if (findit == _nameToId.end())
	{//ó���߰�
		lockId = static_cast<int32>(_nameToId.size());
		_nameToId[name] = lockId;
		_idToName[lockId] = name;
	}
	else {
		lockId = findit->second;
	}

	if (_lockStack.empty() == false)
	{//�̹� ���� ����ִ� ���¿��ٸ�?

		//������ �߰ߵ��� �ʾҴ� ���̽���� ����� ���θ� �ٽ� Ȯ���Ѵ�.
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
	// �ʱ�ȭ��
	const int32 lockCount = static_cast<int32>(_nameToId.size());
	_discoveredOrder = vector<int32>(lockCount, -1);
	_discoveredCount = 0;
	_finished = vector<bool>(lockCount, false);
	_parent = vector<int32>(lockCount, -1);

	for (int32 lockId = 0; lockId < lockCount; lockId++)
	{
		Dfs(lockId);
	}

	//������ �����ٸ� �����Ѵ�.
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
	//��� ���������� ��ȸ�Ѵ�.

	auto findit = _lockHistory.find(here);
	if (findit == _lockHistory.end())
	{//���� �ѹ��� ���� ���� ���ߴ�.
		_finished[here] = true;
		return;
	}

	set<int32>& nextSet = findit->second;
	for (int32 there : nextSet)
	{
		//���� �湮������ ���ٸ� �湮�� ��.
		if (_discoveredOrder[there] == -1)
		{
			_parent[there] = here;
			Dfs(there);
			continue;
		}
		// ���� here�� there���� ���� �߰ߵǾ��ٸ�, there�� here�� �ļ��̴�.(������ ����)
		if (_discoveredOrder[here] < _discoveredOrder[there])
			continue;

		//�����⵵ �ƴϰ�, dfs(there)�� ������� �ʾҴٸ�, there�� here�� �����̴�.(������ ����)
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