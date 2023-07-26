#include "pch.h"
#include "CorePch.h"
#include <iostream>

#include <thread>
#include <atomic>
#include <mutex>
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
//		//compare_exchange_strong의 의사코드
//		/*if (_locked == bExpected)
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

}

