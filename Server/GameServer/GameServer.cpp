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
// TLS
thread_local int32 LThreadId = 0;

void ThreadMain(int32 threadId) {
	LThreadId = threadId;
	
	while (true) {
		cout << "Hi I am Thread" << LThreadId << endl;
		this_thread::sleep_for(1s);
	}
}

int main()
{
	vector<thread> threads;
	for (int32 i = 0; i < 10; i++)
	{
		int32 threadId = i + 1;
		threads.push_back(thread(ThreadMain, threadId));
	}

	for (auto& iter : threads)
	{
		iter.join();
	}

}

