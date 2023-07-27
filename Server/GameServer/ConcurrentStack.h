#pragma once
#include <mutex>
#include <atomic>

template<typename T>
class LockStack {

public:
	LockStack() {};

	LockStack(const LockStack&) = delete;
	LockStack& operator=(const LockStack&) = delete;

	void Push(T& value)
	{
		lock_guard<mutex> lock(_mutex);
		_stack.push(std::move(value));
		_condVar.notify_one();
	} 
	
	bool TryPop(T& value)
	{
		lock_guard<mutex> lock(_mutex);
		if (_stack.empty())
			return false;

		value = std::move(_stack.top());
		_stack.pop();
		return true;
	}

	void WaitPop(T& value)
	{
		unique_lock<mutex> lock(_mutex);
		_condVar.wait(lock, [this]() {
			return _stack.empty() == false;
			});
		value = std::move(_stack.top());
		_stack.pop();

	}

private:
	stack<T> _stack;
	mutex _mutex;
	condition_variable _condVar; //���� ���ٸ� ���Ǻη� ��ٸ��� ó���Ϸ���
};

template<typename T>
class LockFreeStack
{
public:
	struct Node
	{
		Node(const T& value) : data(value)
		{

		}

		T data;
		Node* next;
	};

public:
	// 1. �� ��带 �����
	// 2. �� ����� next�� head���ȴ�.
	// 3. head = �� ���

	void Push(const T& value)
	{
		Node* node = new Node(value);
		node->next = _head;
		
		while (_head.compare_exchange_weak(node->next, node) == false) {

		}

	}

	// 1. head �д´�.
	// 2. head next �д´�.
	// 3. head  = head->next
	// 4. data �����ؼ� ��ȯ
	// 5. ������ ����

	bool TryPop(T& value)
	{
		++_popCount;
		Node* oldHead = _head;

		while (oldHead && _head.compare_exchange_weak(oldHead, oldHead->next)==false);
		{

		}
		if (oldHead == nullptr) {
			--_popCount;
			return false;
		}

		value = oldHead->data;
		TryDelete(oldHead);

		return true;
	}
	void TryDelete(Node* oldHead)
	{
		// ���ܿ� ���� �ִ°�?
		if (_popCount == 1)
		{
			// �� ȥ�ڳ�..
			// �̿� ȥ���ΰ� ��������� �ٸ������͵� ����.
			Node* node = _pendingList.exchange(nullptr);

			if (--_popCount == 0)
			{
				//����� �ְ� ���� = ��������
				// �����ͼ� ����� �����ʹ� �и��� ����
				DeleteNodes(node);
			}
			else if(node)
			{
				// ���� ���������� �ٽ� ���� ����.
				ChainPendingNodeList(node);
			}

			//�� �����ʹ� ����
			delete oldHead;
		}
		else
		{// ���� �ֳ�? �ٷ� �������� �ʰ� ��������
			ChainPendingNode(oldHead);
			--_popCount;
		}
	}

	void ChainPendingNodeList(Node* first, Node* last)
	{
		last->next = _pendingList;
		while (_pendingList.compare_exchange_weak(last->next, first) == false)
		{

		}
	}
	
	void ChainPendingNodeList(Node* node)
	{
		Node* last = node;
		while (last->next)
			last = last->next;
		ChainPendingNodeList(node, last);
	}

	void ChainPendingNode(Node* node)
	{// �ϳ�¥��
		ChainPendingNodeList(node, node);
	}

	static void DeleteNodes(Node* node)
	{
		while (node)
		{
			Node* next = node->next;
			delete node;
			node = next;
		}
	}

private:
	atomic<Node*> _head;

	atomic<uint32> _popCount = 0;// pop�� �������� ������ ����
	atomic<Node*> _pendingList; //�����Ǿ���� ����(ù��° ���)
};