#pragma once
#include <mutex>

class Account 
{


};

class AccountManager
{
public:
	static AccountManager* Instance() {
		static AccountManager instance;
		return &instance;
	}

	Account* GetAccount(int32 id) {
		lock_guard<mutex>guard(m_mutex);
		// 뭔가를 갖고움
		return nullptr;
	}
	void ProcessLogin();

private:
	mutex m_mutex;
	//map<int32, Account*> m_accountsMap;
};

