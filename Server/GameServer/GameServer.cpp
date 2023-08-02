#include "pch.h"
#include "CorePch.h"

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
public:
	int32 _hp = 30;
	int32 _mp = 10;
};


int main()
{
	Knight* knight = xnew<Knight>(100);

	xdelete(knight);
}

