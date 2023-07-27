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
	condition_variable _condVar; //만약 없다면 조건부로 기다릭게 처리하려고
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
	// 1. 새 노드를 만든다
	// 2. 새 노드의 next가 head가된다.
	// 3. head = 새 노드

	void Push(const T& value)
	{
		Node* node = new Node(value);
		node->next = _head;
		
		while (_head.compare_exchange_weak(node->next, node) == false) {

		}

	}

	// 1. head 읽는다.
	// 2. head next 읽는다.
	// 3. head  = head->next
	// 4. data 추출해서 반환
	// 5. 추출노드 삭제

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
		// 나외에 누가 있는가?
		if (_popCount == 1)
		{
			// 나 혼자네..
			// 이왕 혼자인거 삭제예약된 다른데이터도 삭제.
			Node* node = _pendingList.exchange(nullptr);

			if (--_popCount == 0)
			{
				//끼어든 애가 없음 = 삭제진행
				// 이제와서 끼어들어도 데이터는 분리된 형태
				DeleteNodes(node);
			}
			else if(node)
			{
				// 누가 끼어들었으니 다시 갖다 놓자.
				ChainPendingNodeList(node);
			}

			//내 데이터는 삭제
			delete oldHead;
		}
		else
		{// 누가 있네? 바로 삭제하지 않고 삭제예약
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
	{// 하나짜리
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

	atomic<uint32> _popCount = 0;// pop을 실행중인 쓰레드 갯수
	atomic<Node*> _pendingList; //삭제되어야할 노드들(첫번째 노드)
};