#include "pch.h"
#include "CorePch.h"
#include <iostream>

#include <thread>
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

// 소수 구하기.
const int32 MAX_NUMBER = 100'0000;
const int16 iThreadMaxNum = 5;


list<int32> vResult;
mutex m;

void isPrime(int16 iThreadNum)
{
	int32 iMakeMaxNum = MAX_NUMBER / iThreadMaxNum;
	for (int32 i = iThreadNum * iMakeMaxNum; i < iMakeMaxNum * (iThreadNum + 1) ; ++i)
	{
		if (i <= 1)//1보다 작으면 의미없음
			continue;

		if (i % 2 == 0)// 2로 나눠지면 의미없음
			continue;

		bool bSoSu = true;
		for (int32 j = 3; j <= sqrt(i); j += 2)
		{// 루트씌워서 더 작은값으로 나눠서 나눠떨어지면 소수아님
			if (i % j == 0) {
				bSoSu = false;
				break;
			}
		}

		if (bSoSu) {// 성공하면 vector에 담자
			m.lock();
			vResult.push_back(i);
			m.unlock();
		}
	}
	cout << iThreadNum << "번 쓰레드 성공!" << endl;
}

int main()
{
	vector<thread> tVec;
	for (int16 iThreadNum = 0; iThreadNum < iThreadMaxNum; ++iThreadNum)
	{
		tVec.push_back(thread(isPrime, iThreadNum));
	}

	for (auto& iter : tVec) {
		if(iter.joinable())
			iter.join();
	}


	GThreadManager->Join();

	
	vResult.push_back(2);

	vResult.sort();
	
	for (auto& iter : vResult) 
		cout << iter << endl;
	
	cout << "사이즈는 " << vResult.size();


}

