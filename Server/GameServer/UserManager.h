#pragma once
#include <mutex>

class User {

};

class UserManager
{
public:
	static UserManager* Instance() {
		static UserManager Instance;
		return &Instance;
	}

	User* GetUser(int32 id) {
		lock_guard<mutex> guard(m_mutex);
		//¹º°¡ °®°í¿Í
		return nullptr;
	}

	void ProcessSave();

private:
	mutex m_mutex;
	
};

