#include "pch.h"
#include "CorePch.h"
#include <iostream>

#include <thread>
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

#include "PlayerManager.h"
#include "AccountManager.h"


int main()
{
	GThreadManager->Launch([=] 
		{
			while (true)
			{
				cout << "PlayerThenAccount" << endl;
				GPlayerMananger.PlayerThenAccount();

				this_thread::sleep_for(100ms);
			}
		});

	GThreadManager->Launch([=]
		{
			while (true)
			{
				cout << "AccountThenPlayer" << endl;
				GAccountMananger.AccountThenPlayer();

				this_thread::sleep_for(100ms);
			}
		});

	GThreadManager->Join();
}

