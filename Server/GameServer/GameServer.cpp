#include "pch.h"
#include "CorePch.h"
#include <iostream>

#include <thread>
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

#include "RefCounting.h"
#include "Memory.h"

class Knight
{
public:
	Knight()
	{
		cout << "Knight생성" << endl;
	}
	Knight(int32 hp): _hp(hp)
	{
		cout << "Knight생성 인자있음" << endl;
	}
	~Knight()
	{
		cout << "Knight 소멸" << endl;
	}
private:
	int32 _hp = 30;
};


// new operator overloading(Global)

//void* operator new(size_t size)
//{
//	cout << "new" << size << endl;
//	void* ptr = ::malloc(size);
//	return ptr;
//}
//
//void operator delete(void* ptr)
//{
//	cout << "delete" << endl;
//	::free(ptr);
//	return;
//}
//
//void* operator new[](size_t size)
//{
//	cout << "new[]" << size << endl;
//	void* ptr = ::malloc(size);
//	return ptr;
//}
//
//void operator delete[](void* ptr)
//{
//	cout << "delete[]" << endl;
//	::free(ptr);
//	return;
//}


int main()
{
	Knight* knight = xnew<Knight>(100);
	xdelete(knight);

}

