#include "pch.h"
#include "CorePch.h"
#include <iostream>

#include <thread>
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "AccountManager.h"
#include "UserManager.h"

//atomic<bool> bFlag;

atomic<bool> ready;
int32 value;

void Producer() {

	value = 10;
	ready.store(true, memory_order::memory_order_seq_cst);
}

void Consumer() {
	while (ready.load(memory_order::memory_order_seq_cst) == false) {
		;
	}
	cout << value << endl;;
}

int main()
{
#pragma region 기본적인 atomic 사용 함수 + 방법.
	//bFlag = false;

	////bFlag.store(true,memory_order::memory_order_seq_cst);

	////bool val = bFlag.load(memory_order::memory_order_seq_cst);
	////cout << val;
	//{
	//	bool bPrev = bFlag.exchange(true);// exchange는 인자로 채우고싶은 값을받고, return값으로 기존값을 리턴.

	//	//bool bPrev = bFlag.load();// 다른쓰레드가 flab값을 바꿨다면 유효하지 않은 prev값이 될수있음.
	//	//bFlag.store(false);
	//	
	//}
	////CAS Compare and Swap(조건부로 수정하겠다.)
	//{
	//	bool expected = false;
	//	bool desired = true;
	//	bFlag.compare_exchange_strong(expected, desired);
	//	// 위코드의 의사코드
	//	if (bFlag == expected) {
	//		bFlag = desired;
	//		return true;
	//	}
	//	else {
	//		expected = bFlag;
	//		return false;
	//	}
	//}

#pragma endregion

	ready = false;
	value = 0;
	thread t1(Producer);
	thread t2(Consumer);
	t1.join();
	t2.join();


	// memory order 메모리 정책
	// 1) Sequentially Consistant 일관성있다. (memory_order::memory_order_seq_cst)
	// 2) Acquire-Release(acquire, release)
	// 3) Relaxed (relaxed)


	// 1번은 엄격(컴파일러입장에서 최적화 여지가 적다= 직관적)=> 가시성 바로해결.
	// 2번은 
	// Relaxed 자유롭다 최적화 여지가 많다..


}

