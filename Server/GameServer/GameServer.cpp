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

#pragma region 쓰레드 기초 사용법(함수)

void HelloThread()
{
	cout << "Hello Thread" << endl;
}


void HelloThread_2(int32 Num)
{
	cout << Num << endl;
}
#pragma endregion 

#pragma region 동기화1 atomic
//atomic: All Or Nothing 다 실행되거나 다 실행안되거나

//DB
//A라는 유저 인벤에서 집행검빼고
//B라는 유저 인벤에 집행검 추가

//atomic<int32> sum = 0;
//
//void Add()
//{
//	for (int32 i = 0; i < 100'0000; i++) {
//		sum.fetch_add(1);
//		//sum++;
//
//	}
//}
//
//void Sub()
//{
//	for (int32 i = 0; i < 100'0000; i++) {
//		sum.fetch_add(-1);
//
//		//sum--;
//	}
//}
#pragma endregion

#pragma region 동기화2 mutex Lock
// [1][2][3]
// [1][2][3][][][]
//vector<int32> v;
//
////mutual Exclusive (상호배타적)
//mutex m;
//
////RAII (Resource Acquisition is Initialization)
//template<typename T>
//class LockGuard {
//public:
//	LockGuard(T& m)
//	{
//		_mutex = &m;
//		_mutex->lock();
//	}
//
//	~LockGuard()
//	{
//		_mutex->unlock();
//	}
//
//private:
//	T* _mutex;
//};
//
//void Push() {
//	
//	for (int32 i = 0; i < 10000; i++) 
//	{
//		//std::lock_guard<std::mutex> LockGuard(m); //브레이크문에도 자동으로 잠궈줌.+ 풀어줌.
//		std::unique_lock<std::mutex> UniqueLock(m, std::defer_lock);
//		UniqueLock.lock();
//
//		v.push_back(i);
//		
//	}
//}

#pragma endregion 

#pragma region 데드락 1
//void Func1() {
//	for (int32 i = 0; i < 100; i++)
//	{
//		UserManager::Instance()->ProcessSave();
//	}
//}
//
//void Func2() {
//	for (int32 i = 0; i < 100; i++)
//	{
//		AccountManager::Instance()->ProcessLogin();
//	}
//}
#pragma endregion 

#pragma region 스핀락(무작정 대기)

//class SpinLock 
//{
//public:
//	void lock() {
//		//제대로 된 값이 안나옴. 왜?-> 두개의 쓰레드가 같이 while문에 들어가 서로가 먼저 들어갔다고 우기는 상황.
//		// 즉 while문과 _locked는 같은 시간에 작동해야함.-> 이걸 도와주는게 CAS(Compare and Swap)함수
//		bool bExpected = false;
//		bool bDesired = true;
//
//
//		
//		/*if (_locked == bExpected)	//compare_exchange_strong의 의사코드
//		{
//			bExpected = _locked;
//			_locked = bDesired;
//			return true;
//		}
//		else {
//			bExpected = _locked;
//			return false;
//		}*/
//
//		while (_locked.compare_exchange_strong(bExpected, bDesired) == false) {// 이 한줄이 아래의 while문과 대입문을 한번에 처리할수있도록 해준다.
//			bExpected = false;
//
//			//this_thread::sleep_for(std::chrono::milliseconds(100));
//			this_thread::sleep_for(0ms);
//			//this_thread::yield();
//		}
//		/*while (_locked)
//		{
//
//		}
//		_locked = true;*/
//	}
//
//	void unlock() {
//		_locked.store(false);
//	}
//
//
//private:
//	atomic<bool> _locked = false;	// C++에서 volatile 키워드는 최적화 하지마라 라는 의미이다.
//
//};
//
//
//int32 sum = 0;
//mutex m;
//SpinLock spinLock;
//
//void Add() {
//	for (int32 i = 0; i < 10'0000; i++) {
//		lock_guard<SpinLock> guard(spinLock);
//		sum++;
//	}
//}
//
//void Sub() {
//	for (int32 i = 0; i < 10'0000; i++) {
//		lock_guard<SpinLock> guard(spinLock);
//		sum--;
//	}
//}
#pragma endregion

#pragma region 커널오브젝트와 Condition_variable과 Event
//mutex m;
//queue<int32> q;
//HANDLE handle;
//
//// CV는 UserLevel Object (커널 오브젝트와 달리 동일한 프로그램내부에서만 사용가능)
//condition_variable cv;
//
//void Producer() {
//	while (true) {
//
//		// 1) Lock을 잡고
//		// 2) 공유변수값을 수정
//		// 3) Lock을 풀고
//		// 4) 조건변수 통해 다른 쓰레드에 통지
//
//		{
//			unique_lock<mutex> lock(m);
//			q.push(100);
//
//		}
//
//		cv.notify_one(); //wait중인 쓰레드가 있으면 딱 한개를 깨운다.
//
//		//::SetEvent(handle); //handle을 signal 상태로 변경시킨다.
//		
//		
//		//this_thread::sleep_for(1000ms);
//	}
//}
//
//void Consumer() {
//	while (true) {
//		//::WaitForSingleObject(handle, INFINITE);	
//		// auto라서 자동으로 NonSignal상태로 바꿔줌.
//		unique_lock<mutex> lock(m);
//		cv.wait(lock, []() { return q.empty() == false; });
//		// 1) Lock을 잡고
//		// 2) 조건확인
//		// - 만족O => 빠져나와서 이어서 코드실행
//		// - 만족X => Lock을 풀어주고 대기상태
//
//		//그런데 notify_one을 하면 항상 조건식을 만족하는거 아닐까?
//		// 아님. Spurious WakeUp 가짜기상
//		// notify_one할때 lock을 잡고있지 않을수있기때문
//
//		//if (q.empty() == false) 
//		{
//			int32 data = q.front();
//			q.pop();
//			cout << q.size() << endl;
//		}
//	}
//}

#pragma endregion

int64 Calculate() {
	int64 Sum = 0;

	for (int32 i = 0; i < 100'0000; i++)
		Sum += i;

	return Sum;
}

void PromiseWorker(std::promise<string>&& promise)
{
	promise.set_value("Secret Message");
}

void TaskWorker(std::packaged_task<int64(void)> && task)
{
	task();
}

int main()
{

#pragma region 쓰레드 기초 사용법
	//vector<thread> v;
	//
	//for (int32 i = 0; i < 10; i++) {
	//	v.push_back(thread(HelloThread_2, i));
	//}


	//for (int32 i = 0; i < 10; i++) {
	//	if (v[i].joinable())
	//		v[i].join();
	//}

	//cout << "Hello Main" << endl;	//System Call (OS 커널 요청)



	//std::thread t;
	//t = std::thread(HelloThread_2,10);
	//std::thread t(HelloThread); //위 두문장이랑 같은거임.


	//int32 iCount = t.hardware_concurrency(); //컴퓨터에서 사용가능한 쓰레드 수 hint에 가까움
	//auto id2 = t.get_id();	//쓰레드마다 id

	//t.detach();				//std::thread 객체에서 실제 쓰레드를 분리

	//if(t.joinable())		//쓰레드 객체가 사용되고있는지
	//	t.join();
	
	//HelloWorld();
#pragma endregion 

#pragma region 동기화 1 atomic
	/*Add();
	Sub();
	cout << sum << endl;

	thread t1(Add);
	thread t2(Sub);
	t1.join();
	t2.join();
	cout << sum << endl;*/
#pragma endregion

#pragma region 동기화2 mutex Lock
	/*thread t1(Push);
	thread t2(Push);

	t1.join();
	t2.join();

	cout << v.size() << endl;*/
#pragma endregion

#pragma region 데드락 1
	/*thread t1(Func1);
	thread t2(Func2);

	t1.join();
	t2.join();
	
	cout << "Jobs Done!!" << endl;*/
#pragma endregion 

#pragma region 스핀락 (무작정대기)
	/*thread t1(Add);
	thread t2(Sub);

	t1.join();
	t2.join();
	cout << sum << endl;*/
#pragma endregion

#pragma region 커널오브젝트와 Event
	// 커널 오브젝트
	// Usage Count
	// Signal (파란불), Non-Signal (빨간불) << bool
	// Auto / Manual << bool
	//handle = ::CreateEvent(NULL, FALSE, FALSE, NULL);

	/*thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();*/


	//::CloseHandle(handle);
#pragma endregion

	//std::future
	{
		// deffered -> 지연해서 실행하세요
		// async -> 별도의 쓰레드를 만들어서 실행하세요.
		// deffered | async -> 둘중 알아서 골라주세요~
		// future의 장점은 함수한개자체를 특정상황에 쓰레드나 비동기로 처리할수있기때문에 유용.
		std::future<int64> future = std::async(std::launch::async, Calculate);


		int64 iResult = future.get();// 기다렸다가 받아모.
		cout << iResult << endl;
	}

	//std::promise
	{
		// 미래(std::future)에 결과물을 반환 해줄것이라 약속(std::promise) 해줘
		std::promise<string> promise;
		std::future<string> future = promise.get_future();

		thread t(PromiseWorker, std::move(promise));//std::move는 다른쓰레드에게 값을전달

		string message = future.get();
		cout << message << endl;
		t.join();
	}

	//std::packaged_task
	{
		std::packaged_task<int64(void)> task(Calculate);
		std::future<int64> future = task.get_future();

		thread t(TaskWorker,std::move(task));

		int64 sum = future.get();
		cout << sum << endl;

		t.join();
	}

	//결론
	// mutex나 condition_value까지 가지않고 단순한 애들 처리할때
	// 특히나 한번 발생하는 이벤트에 유용.
	// 1) async: 원하는 함수를 비동기적으로 실행.
	// 2) promise: 결과물을 promise를 통해 future에 받아줌.
	// 3) packaged_task: 원하는 함수의 실행결과를 packaged_task를 통해 future로 받아줌.

}

