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


int32 iBufferArray[10000][10000];

int main()
{
	memset(iBufferArray, 0, sizeof(iBufferArray));
	{
		uint64 start = GetTickCount64();
		int64 sum = 0;
		for (int32 i = 0; i < 10000; i++) {
			for (int32 j = 0; j < 10000; j++) {
				sum += iBufferArray[i][j];
			}
		}
		uint64 end = GetTickCount64();
		cout << end - start << endl;
	}

	{
		uint64 start = GetTickCount64();
		int64 sum = 0;
		for (int32 i = 0; i < 10000; i++) {
			for (int32 j = 0; j < 10000; j++) {
				sum += iBufferArray[j][i];
			}
		}
		uint64 end = GetTickCount64();
		cout << end - start << endl;
	}
}

