#include "pch.h"
#include "CorePch.h"
#include <iostream>

#include <thread>
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

bool IsPrime(int number)
{
	if (number <= 1)
		return false;

	if (number == 2 || number == 3)
		return true;

	for (int i = 2; i < number; i++)
	{
		if ((number % i) == 0)
			return false;
	}

	return true;
}

int CountPrime(int start, int end)
{
	int count = 0;

	for (int number = start; number <= end; number++)
	{
		if (IsPrime(number))
			count++;
	}

	return count;
}

int main()
{
	const int MAX_NUMBER = 100'0000;
	vector<thread> threads;

	int coreCount = thread::hardware_concurrency();
	int jobCount = (MAX_NUMBER / coreCount) + 1;

	atomic<int> primeCount = 0;
	for (int i = 0; i < coreCount; i++)
	{
		int start = (i * jobCount) + 1;
		int end = min(MAX_NUMBER, ((i + 1) * jobCount));

		threads.push_back(thread([start, end, &primeCount]()
			{
				primeCount += CountPrime(start, end);
			}));
	}

	for (thread& t : threads)
		t.join();

	cout << primeCount << endl;
	//GThreadManager->Join();

	
	


}

