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


// 가시성, 코드 재배치
int32 x = 0;
int32 y = 0;
int32 r1 = 0;
int32 r2 = 0;

volatile bool bReady;


void Thread_1() {
	while (!bReady)
		;
	y = 1; // Store Y
	r1 = x; //LoadX
}
void Thread_2() {
	while (!bReady)
		;
	x = 1;
	r2 = y;
}

int main()
{
	int32 count = 0;
	while (1) {
		bReady = false;
		count++;

		x = y = r1 = r2 = 0;
		thread t1(Thread_1);
		thread t2(Thread_2);

		bReady = true;

		t1.join();
		t2.join();

		if (r1 == 0 && r2 == 0)
			break;
	}

	cout << count << " 번 만에 빠져나옴!!!!" << endl;
}

